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

#include <sstream>

pthread_t server_thread;
std::string web_root="web";

pthread_mutex_t image_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t image_cond = PTHREAD_COND_INITIALIZER;

CRawImage* image;
unsigned int camTexture=0;

CHeli* heli;
float pitch,roll,yaw,height;

//Kinect-drone autonomous values
bool autonomous=false;
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

double x_size=2;
double y_size=1;
double z_size=3;

int global_argc;
char** global_argv;

long long count = 0;

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

std::string curr_JPEG_string;

void drone_autonomous()
{
	vec3 temp_loc=getLocation();

	if(autonomous&&temp_loc.z!=-1)
	{
		float x_error_new=temp_loc.x-desired_loc.x;
		float y_error_new=temp_loc.y-desired_loc.y;
		float z_error_new=temp_loc.z-desired_loc.z;

		roll=(x_gain_p*x_error_new+x_gain_d*(x_error_old-x_error_new))*20000.0;
		pitch=-(z_gain_p*z_error_new+z_gain_d*(z_error_old-z_error_new))*20000.0;

		x_error_old=x_error_new;
		y_error_old=y_error_new;
		z_error_old=z_error_new;

		double max=10000;
		clamp(roll,-max,max);
		clamp(pitch,-max,max);

		//std::cout<<"BATTERY\t"<<helidata.battery<<std::endl;
		//std::cout<<roll<<"\t"<<x_error_new<<"\t"<<pitch<<"\t"<<z_error_new<<"\tROLLPITCH\n";
	}
}

//Service Client Function Definition
void service_client(msl::socket& client, std::string& message)
{
	//Get Requests
	if(msl::starts_with(message,"GET"))
	{
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
			request="index.html";

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
		else if(msl::ends_with(request,".jpg")||msl::ends_with(request,".jpeg"))
		{
			int pos=request.find('?');

			if(pos!=-1)
				request=request.substr(0,pos);

			mime_type="image/jpeg";
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
		std::stringstream stringstr;

		//Load File
		if(file_request)
		{
			pthread_mutex_lock(&image_mutex);
			loaded=msl::file_to_string(web_root+"/"+request,file);
			pthread_mutex_unlock(&image_mutex);
		}
		else
		{
			if(msl::starts_with(request,"uav/0/goto"))
			{
				for(unsigned int i = 0; i < request.size() ; ++i)
				{
					if( request[i] == '&')
						request[i] = ' ';
				}

				std::cout << request << std::endl;
				stringstr << request;

				stringstr.ignore(request.size(), '=');
				stringstr >> desired_loc.x;

				stringstr.ignore(request.size(), '=');
				stringstr >> desired_loc.y;

				stringstr.ignore(request.size(), '=');
				stringstr >> desired_loc.z;

				loaded = true;

				file = "No message";
			}
			else if(msl::starts_with(request, "uav/0/takeoff"))
			{
				heli->takeoff();
			}
			else if(msl::starts_with(request, "uav/0/land"))
			{
				heli->land();
			}
		}

		if(loaded)
		{
			//std::cout << "file aways " << count++ << std::endl;
			//client<<msl::http_pack_string(file,mime_type);
			//std::cout << "file aways sent " << std::endl;
			pthread_mutex_lock(&image_mutex);
			client<<msl::http_pack_string(file,mime_type);
			//client<<msl::http_pack_string(curr_JPEG_string,mime_type);
			pthread_mutex_unlock(&image_mutex);
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

void* server_thread_function(void*)
{
	pthread_mutex_lock(&image_mutex);

	//Create Server
	msl::socket server("0.0.0.0:8080");
	server.create();

	//Check Server
	if(server)
		std::cout<<"Server started =)"<<std::endl;
	else
		std::cout<<"Server failed =("<<std::endl;

	//Vectors for Clients
	std::vector<msl::socket> clients;
	std::vector<std::string> client_messages;

	//Check for a Connecting Client
	msl::socket client=server.accept();

	//If Client Connected
	if(client)
	{
		clients.push_back(client);
		client_messages.push_back("");
	}

	pthread_mutex_unlock(&image_mutex);

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

	pthread_create(&server_thread,NULL,server_thread_function,NULL);

	heli=new CHeli();

	image=new CRawImage(640,368);

	//image->getSaveNumber();

	msl::start_2d("Parrot");
	pthread_join(server_thread,NULL);

	return 0;
}


void setup()
{
	glGenTextures(1,&camTexture);

	msl::ui_panel_begin(ui_panel_left);
		msl::ui_set_alignment(ui_center);
		msl::ui_spinner_add("x: ",desired_loc.x,-static_cast<float>(x_size),static_cast<float>(x_size));
		msl::ui_spinner_add("y: ",desired_loc.y,0,static_cast<float>(y_size));
		msl::ui_spinner_add("z: ",desired_loc.z,-static_cast<float>(z_size),static_cast<float>(z_size));
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
	if(msl::input_check_pressed(kb_escape)||exit_button_pressed)
	{
		while(!heli->is_landed())
		{
			autonomous=false;
			heli->land();
		}

		delete image;
		delete heli;
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

	float speed=20000.0;

	if(msl::input_check(kb_q))
		yaw=-speed;

	if(msl::input_check(kb_e))
		yaw=speed;

	if(msl::input_check(kb_a))
		roll=-speed;

	if(msl::input_check(kb_d))
		roll=speed;

	if(msl::input_check(kb_w))
		pitch=-speed;

	if(msl::input_check(kb_s))
		pitch=speed;

	if(msl::input_check(kb_down))
		height=speed;

	if(msl::input_check(kb_up))
		height=-speed;

	if(msl::input_check_pressed(kb_enter)||auto_button_pressed)
		autonomous=!autonomous;

	if(autonomous&&heli->is_landed())
		autonomous=false;

	drone_autonomous();
	heli->setAngles(pitch,roll,yaw,height);

	pthread_mutex_lock(&image_mutex);
	msl::remove_file(web_root+"/"+"photo.jpeg");
	heli->renewImage(image);
	//std::cout << "saving image" << std::endl;
	image->saveJPEG(web_root+"/"+"photo.jpeg");
	//std::cout << "image saved" << std::endl;
	pthread_mutex_unlock(&image_mutex);

	pitch=roll=yaw=height=0.0;

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
