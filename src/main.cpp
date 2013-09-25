#include <iostream>
#include "msl/socket.hpp"
#include "msl/socket_util.hpp"
#include "msl/string_util.hpp"
#include "msl/webserver.hpp"
#include "msl/2d.hpp"
#include <string>
#include "falconer.hpp"
#include <vector>
#include "msl/file_util.hpp"
#include "raw_to_jpeg.h"
#include <thread>
#include "kinect/Kinect.hpp"
#include "PDController.hpp"
#include "PIDController.hpp"
#include "kinect/coordinate_system.hpp"

#ifndef __APPLE__
	#include <GL/glew.h>
	#include <GL/glut.h>
#else
	#include <GLEW/glew.h>
	#include <GLUT/glut.h>
#endif

ardrone a;
unsigned int textureId;

void web_server_thread_function();
bool service_client(msl::socket& client,const std::string& message);
msl::webserver server("0.0.0.0:8080",service_client);
std::string make_json();

Kinect kinect;
bool drone_autonomous = false;
vec3 desired_location = vec3(0,0.1,kcs_distance_to_origin);
PDController pdcontroller(desired_location);
PIDController pidcontroller(desired_location);

jpegDestBuffer last_image;

int main()
{
	a.set_video_feed_bottom();
	server.setup();

	if(server.good()&&a.connect(5))
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

	float speed=0.6;
	float pitch=0;
	float roll=0;
	float altitude=0;
	float yaw=0;
	bool moved=false;

	if(msl::input_check(kb_escape))
		exit(0);

	if(msl::input_check_pressed(kb_r))
		a.emergency_mode_toggle();

	if(msl::input_check(kb_w))
	{
		pitch=-speed;
		moved=true;
	}

	if(msl::input_check(kb_s))
	{
		pitch=speed;
		moved=true;
	}

	if(msl::input_check(kb_a))
	{
		roll=-speed;
		moved=true;
	}

	if(msl::input_check(kb_d))
	{
		roll=speed;
		moved=true;
	}

	if(msl::input_check(kb_left))
	{
		yaw=-speed;
		moved=true;
	}

	if(msl::input_check(kb_right))
	{
		yaw=speed;
		moved=true;
	}

	if(msl::input_check(kb_up))
	{
		altitude=speed;
		moved=true;
	}

	if(msl::input_check(kb_down))
	{
		altitude=-speed;
		moved=true;
	}

	if(msl::input_check_pressed(kb_1))
		a.set_video_feed_front();

	if(msl::input_check_pressed(kb_2))
		a.set_video_feed_bottom();

	if(msl::input_check_pressed(kb_space))
		a.land();
	if(msl::input_check_pressed(kb_0))
		a.takeoff();

	if(msl::input_check_pressed(kb_o))
		drone_autonomous = !drone_autonomous;

	if(moved)
	{
		a.manuever(altitude,pitch,roll,yaw);
	}
	else if(drone_autonomous)
	{
		pdcontroller.autonomous_flight(a, kinect);
		pidcontroller.autonomous_flight(a, kinect);
	}

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
	while(true)
	{
		server.update();
		usleep(0);
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
bool service_client(msl::socket& client,const std::string& message)
{
	std::cout<<message<<std::endl;

	//Get Requests
	if(msl::starts_with(message,"GET"))
	{
		//Create Parser
		std::istringstream istr(msl::http_to_ascii(message));

		//Parse the Request
		std::string request;
		istr>>request;
		istr>>request;

		std::cout<<request<<std::endl;

		if(msl::starts_with(request,"/photo.jpeg"))
		{
			last_image=raw_to_jpeg_array(a.video_data(),640,368,3,JCS_RGB);
			std::stringstream jpeg;
			jpeg.write((char *)&last_image.output[0], last_image.output.size());
			client<<msl::http_pack_string(jpeg.str(),"image/jpeg");
			return true;
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
			desired_location.x = x * (kcs_x_field_size/2.0-kcs_buffer);

			parse_sstr.ignore(request.size(), '=');
			parse_sstr >> y;
			//desired_location.y = y * kcs_y_field_size/2;

			parse_sstr.ignore(request.size(), '=');
			parse_sstr >> z;
			desired_location.z = kcs_distance_to_origin+z*(kcs_z_field_size/2.0-kcs_buffer);

			clamp(-kcs_x_field_size/2.0,kcs_x_field_size/2.0,desired_location.x);
			clamp(-kcs_y_field_size/2.0,kcs_y_field_size/2.0,desired_location.y);
			clamp(kcs_distance_from_kinect,kcs_distance_from_kinect+kcs_z_field_size,desired_location.z);

			pdcontroller.set_desired_location(desired_location);

			client<<msl::http_pack_string("Location Recieved","text/plain");
			return true;
		}
		else if(msl::starts_with(request,"/uav/0/land"))
		{
			a.land();
			client<<msl::http_pack_string("Take off","text/plain");
			return true;
		}
		else if(msl::starts_with(request,"/uav/0/takeoff"))
		{
			a.takeoff();
			client<<msl::http_pack_string("land","text/plain");
			return true;
		}
		else if(msl::starts_with(request,"/uav/0/status"))
		{
			client<<msl::http_pack_string(make_json(), "application/json");
			return true;
		}
		else if(msl::starts_with(request,"/uav/0/control"))
		{
			drone_autonomous = !drone_autonomous;
			client<<msl::http_pack_string("changing control","text/plain");
			return true;
		}
	}

	return false;
}

std::string make_json()
{
	std::stringstream sstr;

	sstr << "{\"x\":\""+msl::to_string(kinect.get_location().x/kcs_x_field_size*2.0)+"\",\"y\":\""+msl::to_string(kinect.get_location().y/kcs_y_field_size*2.0)+"\",\"z\":\""+msl::to_string((kinect.get_location().z-kcs_distance_to_origin)/kcs_z_field_size*2.0)+"\",\"robot\":\""+msl::to_string((int)drone_autonomous)+"\"}";

	return sstr.str();
}
