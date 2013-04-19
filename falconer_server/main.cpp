#include <iostream>
#include "msl/socket.hpp"
#include "msl/string_util.hpp"
#include "msl/2d.hpp"
#include <string>
#include "falconer.hpp"
#include "Kinect.hpp"
#include "PDController.hpp"

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

#include <thread>

#include "raw_to_jpeg.h"

#include <sstream>

//Kinect
Kinect kinect;

//Drone
ardrone a;
unsigned int textureId;

//Bool ... for drone flight
bool drone_autonomous = false;

//Desired location for local refrence
vec3 desired_location = vec3(0,0,2);

msl::socket server("0.0.0.0:80");

//Vectors for Clients
std::vector<msl::socket> clients;
std::vector<std::string> client_messages;

uint8_t decoded_bytes[640*360*3];

//Mutexes
std::mutex photo_mutex;
std::mutex drone_mutex;

PDController pdcontroller(kinect, a, desired_location);

//Service Client Function Declaration
void service_client(msl::socket& client,const std::string& message);
void server_update();

void server_setup()
{
	//Create Server
	server.create_tcp();

	//Check Server
	if(server)
		std::cout<<"Server started =)"<<std::endl;
	else
		std::cout<<"Server failed =("<<std::endl;

	while(true)
	{
		server_update();
	}
}

int main()
{
	std::thread serverThread(server_setup);

	msl::start_2d("ardrone",640,360);

	serverThread.join();

	return 0;
}

void setup()
{
	glGenTextures(1, &textureId);

	if(a.connect())
	{
		std::cout<<":)"<<std::endl;
	}
	else
	{
		std::cout<<":("<<std::endl;
		exit(0);
	}

	kinect.start_thread();
}

void server_update()
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
			if(clients[ii].check()>0&&clients[ii].read(&byte,1)==1)
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
			clients[ii].close();
			clients.erase(clients.begin()+ii);
			client_messages.erase(client_messages.begin()+ii);
			--ii;
		}
	}
}

void loop(const double dt)
{
	a.navdata_update();

	kinect.update_location();

	bool moved=false;

	float speed=0.8;
	float pitch=0;
	float roll=0;
	float altitude=0;
	float yaw=0;

	if(msl::input_check(kb_escape))
		exit(0);

	if(msl::input_check_pressed(kb_r))
		a.emergency_mode_toggle();

	if(msl::input_check_pressed(kb_enter))
	{
		drone_autonomous = !drone_autonomous;
	}

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

	if(msl::input_check(kb_q))
	{
		yaw=-speed;
		moved=true;
	}

	if(msl::input_check(kb_e))
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

	if(msl::input_check_pressed(kb_space))
	{
		if(a.flying())
			a.land();
		else
			a.takeoff();
	}

	if(drone_autonomous)
		pdcontroller.autonomous_flight(altitude,pitch,roll,yaw);
	else if(moved)
		a.manuever(altitude,pitch,roll,yaw);
	else
		a.hover();

	a.video_update();

	//photo_mutex.lock();
	//raw_to_jpeg("web/photo.jpeg", a.video_data(), 640, 368, 3, JCS_RGB);
	//photo_mutex.unlock();

	glBindTexture(GL_TEXTURE_2D,textureId);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,640,360,0,GL_RGB,GL_UNSIGNED_BYTE,(GLvoid*)a.video_data());
	glBindTexture(GL_TEXTURE_2D,0);
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
	double height=120;
	if(drone_autonomous)
		height += 40;
	msl::draw_rectangle(-320+width/2.0, 180-height/2.0, width, height,msl::color(0,0,0,0.7));
	msl::draw_text(-msl::window_width/2.0,msl::window_height/2.0,data);
}

//Service Client Function Definition
void service_client(msl::socket& client,const std::string& message)
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

		if(msl::starts_with(request,"BYTES?"))
		{
			std::string response="BYTES?";

			for(unsigned int ii=0;ii<640*360*3;++ii)
				response+=static_cast<char>(a.video_data()[ii]);

			response+='?';

			client<<msl::http_pack_string(response,"text/plain; charset=x-user-defined");
		}
		else
		{
			int pos=request.find('?');

			if(pos!=-1)
				request=request.substr(0,pos);

			//Web Root Variable (Where your web files are)
			std::string web_root="web";

			//Check for Index
			if(request=="")
				request="index.html";

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

			//Check for drone command
			else if(msl::starts_with(request,"uav/"))
				mime_type="drone/command";

			//File Data Variable
			std::string file;

			//Load File
			if(mime_type != "drone/command" && msl::file_to_string(web_root+"/"+request,file))
			{
				if(file == "photo.jpeg")
					{
						//photo_mutex.lock();
						//raw_to_jpeg("web/photo.jpeg", a.video_data(), 640, 368, 3, JCS_RGB);
						client<<msl::http_pack_string(file,mime_type);
						//photo_mutex.unlock();
					}
				else
					client<<msl::http_pack_string(file,mime_type);
			}
			else if (mime_type == "drone/command")
			{
				mime_type="text/plain";

				if(msl::starts_with(request,"uav/0/goto"))
				{
					for(unsigned int i = 0; i < request.size() ; ++i)
					{
						if( request[i] == '&')
							request[i] = ' ';
					}

					std::stringstream parse_sstr;
					std::stringstream output_sstr;

					parse_sstr << request;

					parse_sstr.ignore(request.size(), '=');
					parse_sstr >> desired_location.x;

					parse_sstr.ignore(request.size(), '=');
					parse_sstr >> desired_location.y;

					parse_sstr.ignore(request.size(), '=');
					parse_sstr >> desired_location.z;
					desired_location.z += + kinect._z_field_size / 2;

					pdcontroller.set_desired_location(desired_location);

					client<<msl::http_pack_string("Location Recieved",mime_type);
				}
			}

			//Bad File
			else if(msl::file_to_string(web_root+"/not_found.html",file))
				client<<msl::http_pack_string(file);

			//not_found.html...Not Found?!  >_<
			else
				client.close();
		}
	}

	//Other Requests (Just kill connection...it's either hackers or idiots...)
	else
	{
		client.close();
	}
}
