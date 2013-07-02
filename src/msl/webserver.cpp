//Web Server Source
//	Created By:		Mike Moss
//	Modified On:	06/20/2013

//Required Libraries:
//	wsock32 (windows only)

//Definitions for "webserver.hpp"
#include "webserver.hpp"

//File Utility Header
#include "file_util.hpp"

//Socket Utility Header
#include "socket_util.hpp"

//String Stream Header
#include <sstream>

//String Utility Header
#include "string_util.hpp"

//Time Utility Header
#include "time_util.hpp"

//Constructor (Default)
msl::webserver::webserver(const std::string& address,bool(*user_service_client)(msl::socket& client,const std::string& message)):
	_user_service_client(user_service_client),_socket(address)
{}

//Boolean Operator (Tests if Server is Good)
msl::webserver::operator bool() const
{
	return good();
}

//Not Operator (For Boolean Operator)
bool msl::webserver::operator!() const
{
	return !good();
}

//Good Function (Tests if Server is Good)
bool msl::webserver::good() const
{
	return _socket.good();
}

//Setup Function (Creates Socket)
void msl::webserver::setup()
{
	_socket.create_tcp();
}

//Update Function (Connects Clients and Runs Server)
void msl::webserver::update()
{
	//Check for a Connecting Client
	msl::socket client=_socket.accept();

	//If Client Connected
	if(client.good())
	{
		_clients.push_back(client);
		_client_messages.push_back("");
	}

	//Handle Clients
	for(unsigned int ii=0;ii<_clients.size();++ii)
	{
		//Service Good Clients
		if(_clients[ii].good())
		{
			//Temp
			char byte='\n';

			//Get a Byte
			if(_clients[ii].available()>0&&_clients[ii].read(&byte,1)==1)
			{
				//Add the Byte to Client Buffer
				_client_messages[ii]+=byte;

				//Check for an End Byte
				if(msl::ends_with(_client_messages[ii],"\r\n\r\n"))
				{
					service_client(_clients[ii],_client_messages[ii]);
					_client_messages[ii].clear();
				}
			}
		}

		//Disconnect Bad Clients
		else
		{
			_clients[ii].close();
			_clients.erase(_clients.begin()+ii);
			_client_messages.erase(_client_messages.begin()+ii);
			--ii;
		}
	}

	//Give OS a Break
	usleep(0);
}

//Service Client Function Definition
void msl::webserver::service_client(msl::socket& client,const std::string& message)
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
		if(_user_service_client==NULL||!_user_service_client(client,msl::http_to_ascii(message)))
		{
			//Web Root Variable (Where your web files are)
			std::string web_root="web";

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
			if(msl::file_to_string(web_root+request,file,true))
				client<<msl::http_pack_string(file,mime_type,false);

			//Bad File
			else if(msl::file_to_string(web_root+"/not_found.html",file,true))
				client<<msl::http_pack_string(file);
		}

		//Close Connection
		client.close();
	}

	//Other Requests (Just kill connection...it's either hackers or idiots...)
	else
	{
		client.close();
	}
}