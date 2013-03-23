//Code based on Helisimple2 ... insert full documentation!!!

#include <stdlib.h>
#include "helisimple/CGui.h"
#include "helisimple/CRecognition.h"
#include "helisimple/CHeli.h"
#include "raw_to_jpeg.h"
#include <pthread.h>

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


int i = 0;
int numSaved = 0;
bool stop = false;
CGui* gui;
CRawImage *image;
SDL_Event event;
CRecognition *recognition;
SPixelPosition pos;
bool move = false;
Uint8 lastKeys[10000];
int keyNumber = 10000;
Uint8 *keys = NULL;
CHeli *heli;
float pitch,roll,yaw,height;
SDL_Joystick* joystick;
TJoyState joy,lastJoy;

//Center bool
bool center = false;
vec3 desired_loc = vec3(0,0,2);
double x_error_old=0;
double y_error_old=0;
double z_error_old=0;

double x_size = 2;
double y_size = .5;
double z_size = 3;

double x_gain_p = 1;
double y_gain_p = 1;
double z_gain_p = 1;

double x_gain_d = -25;
double y_gain_d = 10;
double z_gain_d = -25;

void processJoystick()
{
	SDL_JoystickUpdate();
	//printf("Joystick ");
	for (int i = 0;i<SDL_JoystickNumAxes(joystick);i++){
		joy.axis[i] = SDL_JoystickGetAxis (joystick, i);
		//printf("%i ",joy.axis[i]);
		if (fabs(joy.axis[i]) < 20) joy.axis[i] = 0;
	}

	for (int i = 0;i<11;i++){
		 joy.buttons[i+1] =  SDL_JoystickGetButton (joystick,i);
	}
	//printf("\n");
	roll	= joy.axis[0];
	pitch 	= joy.axis[1];
	yaw 	= joy.axis[2];
	height 	= joy.axis[3];

	if (joy.buttons[7] && lastJoy.buttons[7] == false) heli->takeoff();
	if (joy.buttons[8] && lastJoy.buttons[8] == false) heli->land();
	for (int i = 1;i<5;i++){
		if (joy.buttons[i] && lastJoy.buttons[i]==false) heli->switchCamera(i-1);
	}
	lastJoy = joy;
}

void processKeys()
{
	while (SDL_PollEvent(&event)){
		if (event.type == SDL_MOUSEBUTTONDOWN) recognition->learnPixel(&image->data[3*(image->width*event.motion.y + event.motion.x)]);
	}
	keys = SDL_GetKeyState(&keyNumber);
	if (keys[SDLK_ESCAPE]) stop = true;
	if (keys[SDLK_r]) recognition->resetColorMap();
	if (keys[SDLK_RETURN])image->saveBmp();

	if(keys[SDLK_1])
		desired_loc = vec3(0,0,3);

	if(keys[SDLK_2])
		desired_loc = vec3(-0.5,0,3);

	if(keys[SDLK_3])
		desired_loc = vec3(.5,0,3);

	if (keys[SDLK_KP7])  yaw = -20000.0;
	if (keys[SDLK_KP9])  yaw = 20000.0;
	if (keys[SDLK_KP4])  roll = -20000.0;
	if (keys[SDLK_KP6])  roll = 20000.0;
	if (keys[SDLK_KP8])  pitch = -20000.0;
	if (keys[SDLK_KP2])  pitch = 20000.0;
	if (keys[SDLK_KP_PLUS])  height = 20000.0;
	if (keys[SDLK_KP_MINUS])  height = -20000.0;

	/*if (keys[SDLK_t])  yaw = -20000.0;
	if (keys[SDLK_y])  yaw = 20000.0;
	if (keys[SDLK_g])  roll = -20000.0;
	if (keys[SDLK_h])  roll = 20000.0;
	if (keys[SDLK_b])  pitch = -20000.0;
	if (keys[SDLK_n])  pitch = 20000.0;
	if (keys[SDLK_1])  height = 20000.0;
	if (keys[SDLK_2])  height = -20000.0;*/

	if (keys[SDLK_p]) center = true;
	if (keys[SDLK_o]) center = false;

	//changes camera
	if (keys[SDLK_z]) heli->switchCamera(0);
	if (keys[SDLK_x]) heli->switchCamera(1);
	if (keys[SDLK_c]) heli->switchCamera(2);
	if (keys[SDLK_v]) heli->switchCamera(3);

	if (keys[SDLK_q]) heli->takeoff();
	if (keys[SDLK_a]) heli->land();

	memcpy(lastKeys,keys,keyNumber);
}

template <typename T>
void clamp(T& input,const T min,const T max)
{
	if(input<min)
		input=min;

	if(input>max)
		input=max;
}

void processKinect()
{
	vec3 temp_loc = getLocation();

	if(center && temp_loc.z != -1)
	{
		float x_error_new=temp_loc.x - desired_loc.x;
		float y_error_new=temp_loc.y - desired_loc.y;
		float z_error_new=temp_loc.z - desired_loc.z;

		roll = (x_gain_p * x_error_new + x_gain_d * (x_error_old-x_error_new)) * 20000.0;
		pitch = -(z_gain_p * z_error_new + z_gain_d * (z_error_old-z_error_new)) * 20000.0;

		x_error_old=x_error_new;
		y_error_old=y_error_new;
		z_error_old=z_error_new;

		double max=10000;
		clamp(roll,-max,max);
		clamp(pitch,-max,max);

		std::cout<<"BATTERY\t"<<helidata.battery<<std::endl;
		std::cout<<roll<<"\t"<<x_error_new<<"\t"<<pitch<<"\t"<<z_error_new<<"\tROLLPITCH\n";
	}

	return;
}

pthread_t server_thread;
pthread_mutex_t image_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t image_cond = PTHREAD_COND_INITIALIZER;

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

		int pos = request.find('?');
		if(pos != -1)
		{
			request = request.substr(0, pos);
		}

		//std::cout << request << std::endl;

		//Web Root Variable (Where your web files are)
		std::string web_root=".";

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
		else if(msl::ends_with(request,".jpg")||msl::ends_with(request,".jpeg"))
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
		//std::cout<<msl::file_to_string(web_root+"/"+request,file)<<std::endl;

		pthread_mutex_lock(&image_mutex);
		if(msl::file_to_string(web_root+"/"+request,file))
			client<<msl::http_pack_string(file,mime_type);
		pthread_mutex_unlock(&image_mutex);

		//Bad File
		//else if(msl::file_to_string(web_root+"/not_found.html",file))
		//	client<<msl::http_pack_string(file);

		//not_found.html Not Found?!
		//else
		//	client.close();
	}

	//Other Requests (Just kill connection...)
	else
	{
		client.close();
	}
}

void *server_thread_function(void*)
{
	//Added
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
				clients[ii].close();
				clients.erase(clients.begin()+ii);
				client_messages.erase(client_messages.begin()+ii);
				--ii;
			}
		}
	}


}

int main(int argc,char* argv[])
{
	int res = pthread_create(&server_thread, NULL, server_thread_function, NULL);

	startKinectThread(argc, argv, x_size, y_size, z_size, z_size/2+1.5, false);

	//initializing stuff
	heli = new CHeli();
	gui = new CGui(640,368);

	image = new CRawImage(640,368);

	joystick = SDL_JoystickOpen(0);
	fprintf(stdout,"Joystick with %i axes, %i buttons and %i hats initialized.\n",SDL_JoystickNumAxes(joystick),SDL_JoystickNumButtons(joystick),SDL_JoystickNumHats(joystick));

	//this class holds the image
	recognition = new CRecognition();
	image->getSaveNumber();

	while (stop == false){

		//fprintf(stdout,"Angles %.2lf %.2lf %.2lf ",helidata.phi,helidata.psi,helidata.theta);
		//fprintf(stdout,"Speeds %.2lf %.2lf %.2lf ",helidata.vx,helidata.vy,helidata.vz);
		//fprintf(stdout,"Battery %.0lf \n",helidata.battery);
		//fprintf(stdout,"Largest blob %i %i\n",pos.x,pos.y);

		//image capture
		heli->renewImage(image);
		processJoystick();
		processKeys();

		//Added routine --
		processKinect();
		//std::cout << "I am at x: " << getLocation().x << std::endl;
		//std::cout << "I am at y: " << getLocation().y << std::endl;
		//std::cout << "I am at z: " << getLocation().z << std::endl;

		//fprintf(stdout, "Height is %.2lf", height);
		//fprintf(stdout, "Center is %s", (center ? "true" : "false"));

		heli->setAngles(pitch,roll,yaw,height);

		pthread_mutex_lock(&image_mutex);
		image->saveJPEG("photo.jpeg");
		pthread_mutex_unlock(&image_mutex);

		//std::cout << center << std::endl;

		//image->saveBmp("photo.bmp");


		//finding a blob in the image
		pitch=roll=yaw=height=0.0;
		pos = recognition->findSegment(image);

		//turns the drone towards the colored blob
		//yaw = 100*(pos.x-160); //uncomment to make the drone to turn towards a colored target

		//drawing the image, the cross etc.
		image->plotLine(pos.x,pos.y);
		image->plotCenter();
		gui->drawImage(image);
		gui->update();
		i++;

		usleep(20000);
	}

	delete recognition;
	delete heli;
	delete image;
	delete gui;

	return 0;
}

