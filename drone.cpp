//Code based on Helisimple2 ... insert full documentation!!!

#include "helisimple/CHeli.h"
#include "helisimple/raw_to_jpeg.h"
#include <pthread.h>

//2D Header
#include "msl/2d.hpp"

//Kinect libraries
#include "libfree/cyber_kinect.h"

//File Utility Header
#include "msl/file_util.hpp"

//IO Stream Header
#include <iostream>

//Socket Header
#include "msl/socket.hpp"

//Socket Utility Header
#include "msl/socket_util.hpp"

//String Header
#include <string>

//String Utility Header
#include "msl/string_util.hpp"

//Vector Header
#include <vector>

#include <time.h>
#include <sys/time.h>
#include <fstream>

#include <sstream>

#include "falconer.hpp"

//pthread_t server_thread;
std::string web_root="web";

//pthread_mutex_t image_mutex = pthread_mutex_INITIALIZER; //Protects JPEG file written to by saveJPEG and accessed by file_to_string

pthread_t server_thread;

CRawImage* image;
unsigned int camTexture=0;

//pthread_mutex_t drone_command_mutex = //pthread_mutex_INITIALIZER; //Protect heli object
CHeli* heli;

//Kinect-drone autonomous values
bool autonomous=false;

//pthread_mutex_t drone_location_mutex = //pthread_mutex_INITIALIZER; //Protects desired location edited by server client function
vec3 desired_loc=vec3(0,0,2);

double x_error_old=0;
double y_error_old=0;
double z_error_old=0;

double x_gain_p=1;
double y_gain_p=1;
double z_gain_p=1;

double x_gain_d=-50;
double y_gain_d=10;
double z_gain_d=-50;

double x_size=3;
double y_size=1;
double z_size=3;

int global_argc;
char** global_argv;

long long count = 0;

ardrone test;

//Keyboard control bools
bool auto_button_down=false;
bool auto_button_pressed=false;
bool land_button_down=false;
bool land_button_pressed=false;
bool takeoff_button_down=false;
bool takeoff_button_pressed=false;
bool exit_button_down=false;
bool exit_button_pressed=false;

template <typename T>
void clamp(T& input,const T min,const T max)
{
	if(input<min)
		input=min;

	if(input>max)
		input=max;
}

class Drone
{
public:
	Drone();

	void drone_autonomous();
	void zero_out_values();

	float roll;
	float pitch;
	float yaw;
	float height;

};

Drone::Drone()
{
	zero_out_values();
}

void Drone::zero_out_values()
{
	roll=pitch=yaw=height=0;
}

void Drone::drone_autonomous()
{
	vec3 temp_loc=getLocation();

	if(autonomous&&temp_loc.z!=-1)
	{
		//pthread_mutex_lock(&drone_location_mutex);

		float x_error_new=temp_loc.x-desired_loc.x;
		float y_error_new=temp_loc.y-desired_loc.y;
		float z_error_new=temp_loc.z-desired_loc.z;

		//pthread_mutex_unlock(&drone_location_mutex);

		roll=(x_gain_p*x_error_new+x_gain_d*(x_error_old-x_error_new))*20000.0;
		pitch=-(z_gain_p*z_error_new+z_gain_d*(z_error_old-z_error_new))*20000.0;

		x_error_old=x_error_new;
		y_error_old=y_error_new;
		z_error_old=z_error_new;

		double max=1000;
		clamp(roll,-max,max);
		clamp(pitch,-max,max);

		//std::cout<<"BATTERY\t"<<helidata.battery<<std::endl;
		//std::cout<<roll<<"\t"<<x_error_new<<"\t"<<pitch<<"\t"<<z_error_new<<"\tROLLPITCH\n";
	}
}

//Global drone object for drone values for angles and height
Drone drone;

std::string make_json()
{
	std::stringstream json;

	vec3 curr_location = getLocation();

	json << "{ \n";
	json << "current_x : " << curr_location.x << ",\n";
	json << "current_y : " << curr_location.y << ",\n";
	json << "current_z : " << curr_location.z << "\n";
	json << "}";

	return json.str();
}

//Service Client Function Definition
void service_client(msl::socket& client, std::string& message)
{
	//Get Requests
	if(msl::starts_with(message,"GET"))
	{
		std::cout << message << std::endl;
		//Convert Request
		std::string request=msl::http_to_ascii(message);

		//Remove "GET /"
		for(unsigned int ii=0;ii<5;++ii)
			if(request.size()>0)
				request.erase(request.begin());

		//Remove " HTTP/1.1 "
		for(unsigned int ii=0;ii<10;++ii)
			if(request.size()>0)
				request.erase(request.end()-1);

		//Check for Index
		if(request=="")
			request="cyberAlaskaInterface.html";

		int pos=request.find('?');

		if(pos!=-1)
			request=request.substr(0,pos);

		//Mime Type Variable (Default plain text)
		std::string mime_type="text/plain";

		bool file_request = false;

		//Check for Code Mime Type
		if(msl::ends_with(request,".js"))
		{
			mime_type="application/x-javascript";
			file_request = true;
		}

		//Check for Images Mime Type
		else if(msl::ends_with(request,".gif"))
		{
			mime_type="image/gif";
			file_request = true;
		}
		else if(msl::ends_with(request,".jpg") || msl::ends_with(request,".jpeg"))
		{
			//std::cout << request << std::endl;
			mime_type="image/jpeg";
			//request = "photo.jpeg";
			file_request = true;
		}
		else if(msl::ends_with(request,".png"))
		{
			mime_type="image/png";
			file_request = true;
		}
		else if(msl::ends_with(request,".tiff"))
		{
			mime_type="image/tiff";
			file_request = true;
		}
		else if(msl::ends_with(request,".svg"))
		{
			mime_type="image/svg+xml";
			file_request = true;
		}
		else if(msl::ends_with(request,".ico"))
		{
			mime_type="image/vnd.microsoft.icon";
			file_request = true;
		}

		//Check for Text Mime Type
		else if(msl::ends_with(request,".css"))
		{
			mime_type="text/css";
			file_request = true;
		}
		else if(msl::ends_with(request,".htm")||msl::ends_with(request,".html"))
		{
			mime_type="text/html";
			file_request = true;
		}

		//File Data Variable
		std::string file;

		bool loaded = false;

		std::stringstream parse_sstr;
		std::stringstream output_sstr;

		if(file_request)
		{
			//std::cout << "Load this image" << std::endl;
			//pthread_mutex_lock(&image_mutex);
			//std::cout << "Loading this image" << std::endl;
			loaded=msl::file_to_string(web_root+"/"+request,file);
			//std::cout<<"FILE LENGTH:\t"<<file.size()<<std::endl;
			//pthread_mutex_unlock(&image_mutex);
		}
		else
		{
			loaded = true;

			if(msl::starts_with(request,"uav/0/goto"))
			{
				for(unsigned int i = 0; i < request.size() ; ++i)
				{
					if( request[i] == '&')
						request[i] = ' ';
				}

				parse_sstr << request;

				parse_sstr.ignore(request.size(), '=');
				parse_sstr >> desired_loc.x;

				parse_sstr.ignore(request.size(), '=');
				parse_sstr >> desired_loc.y;

				parse_sstr.ignore(request.size(), '=');
				parse_sstr >> desired_loc.z;
				desired_loc.z += + kinect._z_field_size / 2;\

				PDController.set_desired_location(desired_loc);

				output_sstr << "Location recieved";
			}
			else if(msl::starts_with(request, "uav/0/takeoff"))
			{
				drone_mutex.lock();
				a->takeoff();
				drone_mutex.unlock();

				output_sstr << "takeoff recieved";
			}
			else if(msl::starts_with(request, "uav/0/land"))
			{
				drone_mutex.lock();
				heli->land();
				drone_mutex.unlock();

				output_sstr << "land recieved";
			}
			else if(msl::starts_with(request, "uav/0/status"))
			{
				//output_sstr << make_json();
			}
			else
			{
				std::cout << request << std::endl;
			}

			file += output_sstr.str();
		}

		if(loaded)
		{
			//std::cout<<"BYTES SENT:\t"<<client.write((void*)msl::http_pack_string(file,mime_type).c_str(),msl::http_pack_string(file,mime_type).size())<<std::endl;
			//pthread_mutex_lock(&image_mutex);
			client.write((void*)msl::http_pack_string(file,mime_type).c_str(),msl::http_pack_string(file,mime_type).size());
			//pthread_mutex_unlock(&image_mutex);
		}

		//not_found.html Not Found?!
		else
		{
			client.close();
		}
	}

	//Other Requests (Just kill connection...)
	else
	{
		//std::cout << message << std::endl;
		client.close();
	}
}

//Create Server
msl::socket server("0.0.0.0:80");

//Vectors for Clients
std::vector<msl::socket> clients;
std::vector<std::string> client_messages;

void* server_thread_function(void*)
{
	//Check for a Connecting Client
	msl::socket client=server.accept();

	//If Client Connected
	if(client)
	{
		clients.push_back(client);
		client_messages.push_back("");
	}

	//Be a server...forever...
	while(true)
	{
		//Check for a Connecting Client
		msl::socket client=server.accept();

		//If Client Connected
		if(client)
		{
			clients.push_back(client);
			client_messages.push_back("");
		}

		//Handle Clients
		for(unsigned int ii=0;ii<clients.size();++ii)
		{
			//Service Good Clients
			if(clients[ii])
			{
				//Temp
				char byte='\n';

				//Get a Byte
				if(clients[ii].check()>0&&clients[ii].read(&byte,1))
				{
					//End Byte (Not really...but it works for what we're doing...)
					if(byte=='\n')
					{
						service_client(clients[ii],client_messages[ii]);
						client_messages[ii].clear();
					}

					//Other Bytes
					else
					{
						client_messages[ii]+=byte;
					}
				}
			}

			//Disconnect Bad Clients
			else
			{
				//std::cout << "client closed" << clients.size() << std::endl;
				clients[ii].close();
				clients.erase(clients.begin()+ii);
				client_messages.erase(client_messages.begin()+ii);
				--ii;
			}
		}
	}

	return NULL;
}

int main(int argc,char* argv[])
{
	//global_argc = argc;
	//global_argv = argv;

	test.connect();

	heli=new CHeli();

	image=new CRawImage(640,368);

	pthread_create(&server_thread,NULL,server_thread_function,NULL);

	msl::start_2d("Parrot");

	pthread_join(server_thread,NULL);
	return 0;
}


void setup()
{
		server.create_tcp();

		//Check Server
	if(server)
		std::cout<<"Server started =)"<<std::endl;
	else
		std::cout<<"Server failed =("<<std::endl;

		glGenTextures(1,&camTexture);

	float myTheta = helidata.theta;
	std::cout << myTheta << std::endl;

	msl::ui_panel_begin(ui_panel_left);
		msl::ui_set_alignment(ui_center);
		msl::ui_spinner_add("x: ",desired_loc.x,-static_cast<float>(x_size),static_cast<float>(x_size));
		msl::ui_spinner_add("y: ",desired_loc.y,0,static_cast<float>(y_size));
		msl::ui_spinner_add("z: ",desired_loc.z,0,static_cast<float>(z_size));
		msl::ui_separator_add();
		msl::ui_button_add("autonomous",auto_button_down,auto_button_pressed);
		msl::ui_separator_add();
		msl::ui_button_add("take off",takeoff_button_down,takeoff_button_pressed);
		msl::ui_button_add("land",land_button_down,land_button_pressed);
		msl::ui_separator_add();
		msl::ui_button_add("exit",exit_button_down,exit_button_pressed);
	msl::ui_panel_end();

	startKinectThread(global_argc,global_argv,x_size,y_size,z_size,z_size/2+1.5,false);
}

void loop(const double dt)
{
	//server_thread_function(NULL);
	//pthread_mutex_lock(&drone_command_mutex);

	if(msl::input_check_pressed(kb_escape)||exit_button_pressed)
	{
		while(!heli->is_landed())
		{
			autonomous=false;
			heli->land();
		}

		//delete image;
		//delete heli;
		exit(0);
	}

	if(msl::input_check_pressed(kb_space))
	{
		if(heli->is_landed())
			heli->takeoff();
		else
			heli->land();
	}

	if(land_button_pressed)
		heli->land();

	if(takeoff_button_pressed)
		heli->takeoff();

	if(msl::input_check_pressed(kb_c))
		heli->switchCamera(2);

	if(msl::input_check_pressed(kb_v))
		heli->switchCamera(3);

	//pthread_mutex_unlock(&drone_command_mutex);


	float speed=20000.0;

	if(msl::input_check(kb_q))
		drone.yaw=-speed;

	if(msl::input_check(kb_e))
		drone.yaw=speed;

	if(msl::input_check(kb_a))
		drone.roll=-speed;

	if(msl::input_check(kb_d))
		drone.roll=speed;

	if(msl::input_check(kb_w))
		drone.pitch=-speed;

	if(msl::input_check(kb_s))
		drone.pitch=speed;

	if(msl::input_check(kb_down))
		drone.height=speed;

	if(msl::input_check(kb_up))
		drone.height=-speed;

	if(msl::input_check_pressed(kb_enter)||auto_button_pressed)
		autonomous=!autonomous;

	//pthread_mutex_lock(&drone_command_mutex);
	if(autonomous&&heli->is_landed())
		autonomous=false;
	//pthread_mutex_unlock(&drone_command_mutex);

	drone.drone_autonomous();

	//pthread_mutex_lock(&drone_command_mutex);
	heli->setAngles(drone.pitch,drone.roll,drone.yaw,drone.height);
	//pthread_mutex_unlock(&drone_command_mutex);

	//pthread_mutex_lock(&drone_command_mutex);
	//heli->renewImage(image);
	test.video_update();
	image->data=test.video_data();
	//pthread_mutex_unlock(&drone_command_mutex);

	//pthread_mutex_lock(&image_mutex);
	image->saveJPEG(web_root+"/"+"photo.jpeg");
	//pthread_mutex_unlock(&image_mutex);

	drone.zero_out_values();

	static std::ofstream ostr;
	ostr.open("log_new.txt");

	timeval tim;
	gettimeofday(&tim, NULL);
	double t = tim.tv_sec+(tim.tv_usec/1000000.0);
	static double start_time = t;
	t-=start_time;

	ostr << t;
	ostr << " battery " << helidata.battery;
	ostr << " theta " << helidata.theta;
	ostr << " phi " << helidata.phi;
	ostr << " psi " << helidata.psi;
	ostr << " vx " << helidata.vx;
	ostr << " vy " << helidata.vy;
	ostr << " vz " << helidata.vz << std::endl;

	/*glBindTexture(GL_TEXTURE_2D,camTexture);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,image->width,image->height,0,GL_RGB,GL_UNSIGNED_BYTE,image->data);*/
}

void draw()
{
	//msl::sprite cam(web_root+"/"+"photo.jpeg");
	//cam.draw(0,0);

	/*glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
		glTexCoord2d(0,0);
		glVertex2d(-320,240);
		glTexCoord2d(1,0);
		glVertex2d(320,240);
		glTexCoord2d(1,1);
		glVertex2d(320,-240);
		glTexCoord2d(0,1);
		glVertex2d(-320,-240);
	glEnd();
	glDisable(GL_TEXTURE_2D);*/

	if(autonomous)
		msl::draw_circle(0,0,30,msl::color(1,0,0));
}
