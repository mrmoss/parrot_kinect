//Web Server Ox Source
//	Created By:		Mike Moss
//	Modified On:	09/24/2013

//C++11 Only

//Required Libraries:
//	wsock32 (windows only)

//Definitions for "webserver_Ox.hpp"
#include "webserver_Ox.hpp"

//File Utility Header
#include "file_util.hpp"

//Socket Utility Header
#include "socket_util.hpp"

//String Stream Header
#include <sstream>

//String Utility Header
#include "string_util.hpp"

//Static Global Service Client Function
void service_client(msl::socket client,const std::string&message,const std::string web_directory,bool(*user_service_client)(msl::socket& client,const std::string& message))
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

		//If User Options Fail
		if(user_service_client==NULL||!user_service_client(client,msl::http_to_ascii(message)))
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
			if(msl::file_to_string(web_directory+request,file,true))
			{
				std::string response_str=msl::http_pack_string(file,mime_type,false);
				client.write(response_str.c_str(),response_str.size(),120000);
			}

			//Bad File
			else if(msl::file_to_string(web_directory+"/not_found.html",file,true))
			{
				std::string response_str=msl::http_pack_string(file);
				client.write(response_str.c_str(),response_str.size(),120000);
			}

			//No Bad File
			else
			{
				std::string response_str=msl::http_pack_string("sorry...");
				client.write(response_str.c_str(),response_str.size(),120000);
			}
		}
	}

	//Other Requests (Just kill connection...it's either hackers or idiots...)
	else
	{
			client.close();
	}
}

//Static Global Client Thread Function
static void client_thread(msl::socket client,const std::string web_directory,bool(*user_service_client)(msl::socket& client,const std::string& message))
{
	//Client Message Buffer
	std::string message="";

	//Keep Getting Messages
	while(true)
	{
		//Dead Check Variable
		bool dead=false;

		//Get a Byte
		while(client.good()&&client.available()>0)
		{
			//Temp
			char byte='\n';

			//Get a Byte
			if(client.read(&byte,1)==1)
			{
				//Add the Byte to Client Buffer
				message+=byte;

				//Check for an End Byte
				if(msl::ends_with(message,"\r\n\r\n"))
				{
					service_client(client,message,web_directory,user_service_client);
					message.clear();
				}
			}

			//If There are Bytes to Read But None are Readable then Client is "dead"
			else
			{
				dead=true;
				break;
			}
		}

		//Disconnect Bad Clients
		if(!client.good()||dead)
		{
			client.close();
			return;
		}

		//Give OS a Break
		usleep(0);
	}
}

//Constructor (Default)
msl::Ox::webserver::webserver(const std::string& address,bool(*user_service_client)(msl::socket& client,const std::string& message),
	const std::string& web_directory):_user_service_client(user_service_client),_socket(address),_web_directory(web_directory)
{}

//Boolean Operator (Tests if Server is Good)
msl::Ox::webserver::operator bool() const
{
	return good();
}

//Not Operator (For Boolean Operator)
bool msl::Ox::webserver::operator!() const
{
	return !good();
}

//Good Function (Tests if Server is Good)
bool msl::Ox::webserver::good() const
{
	return _socket.good();
}

//Setup Function (Creates Socket)
void msl::Ox::webserver::setup()
{
	_socket.create_tcp();
}

//Update Function (Connects Clients and Runs Server)
void msl::Ox::webserver::update()
{
	//Check for a Connecting Client
	msl::socket client=_socket.accept();

	//If Client Connected
	if(client.good())
	{
		//Keeping an Array Of Threads Has Been Disabled But Left in Just Incase
		//	the New Standard Allows for Cancelling Threads...
		//_threads.push_back(new std::thread(client_thread,client,_web_directory,_user_service_client));
		//_threads.back()->detach();

		std::thread* new_client_thread=new std::thread(client_thread,client,_web_directory,_user_service_client);
		new_client_thread->detach();
	}

	//Give OS a Break
	usleep(0);
}

//Close Function (Closes Server) (Warning!!!  This doesn't close all the threads, there is no way to kill a running joined thread in C++11...yet...)
void msl::Ox::webserver::close()
{
	//Delete Threads
	for(auto ii:_threads)
		delete ii;

	//Close Server
	_socket.close();
}