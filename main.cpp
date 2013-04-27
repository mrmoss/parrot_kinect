#include <iostream>
#include "msl/socket.hpp"
#include "msl/socket_util.hpp"
#include "msl/string_util.hpp"
#include "msl/2d.hpp"
#include <string>
#include "falconer.hpp"
#include <vector>
#include "msl/file_util.hpp"
#include "raw_to_jpeg.h"
#include <thread>
#include "kinect/Kinect.hpp"
#include "PDController.hpp"
#include "kinect/coordinate_system.hpp"

ardrone a;
unsigned int textureId;

msl::socket server("0.0.0.0:80");
void web_server_thread_function();
void service_client(msl::socket& client,const std::string& message);
std::string make_json();

Kinect kinect;
bool drone_autonomous = false;
vec3 desired_location = vec3(0,0,2);
PDController pdcontroller(desired_location);

int main()
{
	server.create_tcp();

	if(server&&a.connect())
	{
		std::cout<<":)"<<std::endl;
	}
	else
	{
		std::cout<<":("<<std::endl;
		exit(0);
	}

	std::thread web_server_thread(web_server_thread_function);

	kinect.start_thread();

	msl::start_2d("ardrone",640,360);

	return 0;
}

void setup()
{
	glGenTextures(1, &textureId);
}

void loop(const double dt)
{
	a.navdata_update();

	float speed=-0.8;
	float pitch=0;
	float roll=0;
	float altitude=0;
	float yaw=0;

	if(msl::input_check(kb_escape))
		exit(0);

	if(msl::input_check_pressed(kb_r))
		a.emergency_mode_toggle();

	if(msl::input_check(kb_w))
		pitch=-speed;

	if(msl::input_check(kb_s))
		pitch=speed;

	if(msl::input_check(kb_a))
		roll=-speed;

	if(msl::input_check(kb_d))
		roll=speed;

	if(msl::input_check(kb_q))
		yaw=speed;

	if(msl::input_check(kb_e))
		yaw=-speed;

	if(msl::input_check(kb_up))
		altitude=-speed;

	if(msl::input_check(kb_down))
		altitude=speed;

	if(msl::input_check_pressed(kb_space))
	{
		if(a.flying())
			a.land();
		else
			a.takeoff();
	}

	if(msl::input_check_pressed(kb_o))
		drone_autonomous = !drone_autonomous;

	if(drone_autonomous)
		pdcontroller.autonomous_flight(a, kinect);
	else
		a.manuever(altitude,pitch,roll,yaw);

	a.video_update();

	glBindTexture(GL_TEXTURE_2D,textureId);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,640,360,0,GL_RGB,GL_UNSIGNED_BYTE,(GLvoid*)a.video_data());
	glBindTexture(GL_TEXTURE_2D,0);

	kinect.update_location();
}

void draw()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,textureId);

	glBegin(GL_QUADS);
		glTexCoord2f(0,0);
		glVertex2f(-320,180);
		glTexCoord2f(1,0);
		glVertex2f(320,180);
		glTexCoord2f(1,1);
		glVertex2f(320,-180);
		glTexCoord2f(0,1);
		glVertex2f(-320,-180);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	std::string data;
	data+="Battery:\t"+msl::to_string(a.battery_percent())+"%\n";
	data+="Pitch:\t\t"+msl::to_string(a.pitch())+"\n";
	data+="Roll:\t\t"+msl::to_string(a.roll())+"\n";
	data+="Yaw:\t\t"+msl::to_string(a.yaw())+"\n";
	data+="Altitude:\t"+msl::to_string(a.altitude())+"\n";

	data+="X Location:\t"+msl::to_string(kinect.get_location().x) + "\n";
	data+="Y Location:\t"+msl::to_string(kinect.get_location().y) + "\n";
	data+="Z Location:\t"+msl::to_string(kinect.get_location().z) + "\n";

	if(drone_autonomous)
	{
		data+="X Desired Location:\t"+msl::to_string(desired_location.x) + "\n";
		data+="Y Desired Location:\t"+msl::to_string(desired_location.y) + "\n";
		data+="Z Desired Location:\t"+msl::to_string(desired_location.z) + "\n";
	}

	if(a.emergency_mode())
		data+="Emergency Mode\n";

	if(a.low_battery())
		data+="Low Battery\n";

	if(!a.motors_good())
		data+="Bad Motor\n";

	if(a.flying())
		data+="Flying\n";
	else
		data+="Landed\n";

	data+="Server:\t"+msl::to_string((bool)server)+"\n";

	double width=180;
	double height=160;
	//if(drone_autonomous)
		//height += 40;
	msl::draw_rectangle(-320+width/2.0, 180-height/2.0, width, height,msl::color(0,0,0,0.7));
	msl::draw_text(-msl::window_width/2.0,msl::window_height/2.0,data);
}

void web_server_thread_function()
{
	std::vector<msl::socket> clients;
	std::vector<std::string> client_messages;

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
				while(clients[ii].check()>0&&clients[ii].read(&byte,1)==1)
				{
					//Add the Byte to Client Buffer
					client_messages[ii]+=byte;

					//Check for an End Byte
					if(msl::ends_with(client_messages[ii],"\r\n\r\n"))
					{
						service_client(clients[ii],client_messages[ii]);
						client_messages[ii].clear();
					}
				}
			}

			//Disconnect Bad Clients
			else
			{
				clients[ii].close();
				clients.erase(clients.begin()+ii);
				client_messages.erase(client_messages.begin()+ii);
				--ii;
			}
		}
	}
}

template <typename T> void clamp(const T& min,const T& max,T& value)
{
	if(value<min)
		value=min;

	if(value>max)
		value=max;

	return value;
}

//Service Client Function Definition
void service_client(msl::socket& client,const std::string& message)
{
	//Get Requests
	if(msl::starts_with(message,"GET"))
	{
		//Create Parser
		std::istringstream istr(msl::http_to_ascii(message));

		//Parse the Request
		std::string request;
		istr>>request;
		istr>>request;

		//Web Root Variable (Where your web files are)
		std::string web_root="web";

		if(msl::starts_with(request,"/photo.jpeg"))
		{
			std::stringstream jpeg;
			jpegDestBuffer jpeg_image = raw_to_jpeg_array(a.video_data(), 640, 368, 3, JCS_RGB);
			jpeg.write((char *)&jpeg_image.output[0], jpeg_image.output.size());
			client<<msl::http_pack_string(jpeg.str(),"image/jpeg");
		}
		else if(msl::starts_with(request,"/uav/0/goto"))
		{
			for(unsigned int i = 0; i < request.size() ; ++i)
			{
				if( request[i] == '&')
					request[i] = ' ';
			}

			std::stringstream parse_sstr;

			parse_sstr << request;

			//temps for math
			double x = 0;
			double y = 0;
			double z = 0;

			parse_sstr.ignore(request.size(), '=');
			parse_sstr >> x;
			desired_location.x = x * kcs_x_field_size/2;

			parse_sstr.ignore(request.size(), '=');
			parse_sstr >> y;
			desired_location.y = y * kcs_y_field_size/2;

			parse_sstr.ignore(request.size(), '=');
			parse_sstr >> z;
			desired_location.z = kcs_distance_to_origin+z*kcs_z_field_size/2;

			clamp(-kcs_x_field_size/2,kcs_x_field_size/2,desired_location.x);
			clamp(-kcs_y_field_size/2,kcs_y_field_size/2,desired_location.y);
			clamp(kcs_distance_from_kinect,kcs_distance_from_kinect+kcs_z_field_size,desired_location.z);

			pdcontroller.set_desired_location(desired_location);

			client<<msl::http_pack_string("Location Recieved","text/plain");
		}
		else if(msl::starts_with(request,"/uav/0/land"))
		{
			a.land();
		}
		else if(msl::starts_with(request,"/uav/0/takeoff"))
		{
			a.takeoff();
		}
		else if(msl::starts_with(request,"/uav/0/status"))
		{
			client<<msl::http_pack_string(make_json(), "application/json");
		}
		else
		{
			//Check for Index
			if(request=="/")
				request="/index.html";

			//Mime Type Variable (Default plain text)
			std::string mime_type="text/plain";

			//Check for Code Mime Type
			if(msl::ends_with(request,".js"))
				mime_type="application/x-javascript";

			//Check for Images Mime Type
			else if(msl::ends_with(request,".gif"))
				mime_type="image/gif";
			else if(msl::ends_with(request,".jpeg"))
				mime_type="image/jpeg";
			else if(msl::ends_with(request,".png"))
				mime_type="image/png";
			else if(msl::ends_with(request,".tiff"))
				mime_type="image/tiff";
			else if(msl::ends_with(request,".svg"))
				mime_type="image/svg+xml";
			else if(msl::ends_with(request,".ico"))
				mime_type="image/vnd.microsoft.icon";

			//Check for Text Mime Type
			else if(msl::ends_with(request,".css"))
				mime_type="text/css";
			else if(msl::ends_with(request,".htm")||msl::ends_with(request,".html"))
				mime_type="text/html";

			//File Data Variable
			std::string file;

			//Load File
			if(msl::file_to_string(web_root+request,file))
				client<<msl::http_pack_string(file,mime_type);

			//Bad File
			else if(msl::file_to_string(web_root+"/not_found.html",file))
				client<<msl::http_pack_string(file);
		}
	}

	//Close Connection
	client.close();
}

std::string make_json()
{
	std::stringstream sstr;

	sstr << "{ \n";
	sstr << "\"x\":" << 2;
	sstr << "\n }";

	return sstr.str();
}
