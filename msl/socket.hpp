//Socket Header
//	Created By:		Mike Moss
//	Modified On:	03/12/2013

//Required Libraries:
//	wsock32 (windows only)

//Begin Define Guards
#ifndef MSL_SOCKET_H
#define MSL_SOCKET_H

//String Header
#include <string>

//String Stream Header
#include <sstream>

//Windows Dependices
#if(defined(_WIN32)&&!defined(__CYGWIN__))
	#include <winsock.h>
	#pragma comment(lib,"Ws2_32.lib")

	#if(!defined(socklen_t))
		typedef int socklen_t;
	#endif

//Unix Dependices
#else
	#include <netinet/in.h>
	#include <unistd.h>
	#include <fcntl.h>

	#ifndef SOCKET
		#define SOCKET unsigned int
		#define INVALID_SOCKET (SOCKET)(~0)
		#define SOCKET_ERROR (-1)
	#endif
#endif

//MSL Namespace
namespace msl
{
	//End Line Variable(std::endl doesn't work)
	extern const char endl;

	//Socket Class Pre-Declaration(For msl::ipv4)
	class socket;

	//IPv4 Address Class Declaration
	class ipv4
	{
		public:
			//Constructor (Default)
			ipv4(const unsigned char ip[4]=NULL,const unsigned short port=0);

			//Copy Constructor
			ipv4(const msl::ipv4& copy);

			//Copy Assignment Operator
			msl::ipv4& operator=(const msl::ipv4& copy);

			//Build Function (Returns Raw Socket Address)
			sockaddr_in build() const;

			//String Accessor (X.X.X.X:PORT)
			std::string str() const;

			//Socket Class Friend
			friend class msl::socket;

		private:
			//Member Variables
			unsigned char _ip[4];
			unsigned short _port;
	};

	//Socket Class Declaration (NOT SURE IF BEING A STD::OSTREAM CHILD IS THE WAY TO GO HERE)
	class socket:public std::ostream
	{
		public:
			//Constructor (Default)
			socket(const std::string& address="0.0.0.0:0");

			//Copy Constructor
			socket(const msl::socket& copy);

			//Copy Assignment Operator
			socket& operator=(const msl::socket& copy);

			//Boolean Operator (Tests If Socket Is Good)
			operator bool() const;

			//Not Operator (For Boolean Operator)
			bool operator!() const;

			//Create Functions (Hosts a Socket Locally)
			void create_tcp();
			void create_udp(const unsigned int buffersize);

			//Connect Functions (Connects to a Remote Socket)
			void connect_tcp();
			void connect_udp();

			//Close Function (Closes a Local Socket)
			void close();

			//Accept Function (Accepts a Remote Connection to a Local Socket)
			msl::socket accept();

			//Read Function (Returns True if Read was Successful)
			int read(void* buffer,const unsigned int size,const int flags=0) const;

			//Write Function (Returns True if Write was Successful)
			int write(void* buffer,const unsigned int size,const int flags=0) const;

			//Check Function (Checks How Many Bytes there are to be Read, -1 on Error)
			int check() const;

			//IP Address Accessor (Read Only)
			msl::ipv4 ip() const;

			//System Socket Accessor
			SOCKET system_socket() const;

			//Stream Out Operator
			template <typename T> friend msl::socket& operator<<(msl::socket& lhs,const T& rhs);

		private:
			//Member Variables
			msl::ipv4 _address;
			SOCKET _socket;
			bool _hosting;
	};

	//Socket Class Stream Operator (Templated Function)
	template <typename T> msl::socket& operator<<(msl::socket& lhs,const T& rhs)
	{
		//Create a String Stream
		std::ostringstream ostr;

		//Put in Data
		ostr<<rhs;

		//Write Data
		lhs.write(reinterpret_cast<void*>(const_cast<char*>(ostr.str().c_str())),ostr.str().size());

		//Return Stream
		return lhs;
	}
}

//Socket Create Function (Hosts a Socket Locally)
SOCKET socket_create(const msl::ipv4 ip,const unsigned int time_out=0,const bool UDP=false,const unsigned int buffersize=200);

//Socket Connection Function (Connects to a Remote Socket)
SOCKET socket_connect(const msl::ipv4 ip,const unsigned int time_out=0,const bool UDP=false);

//Socket Accept Function (Accepts a Remote Connection to a Local Socket)
SOCKET socket_accept(const SOCKET socket,msl::ipv4& client_ip,const unsigned int time_out=0);

//Socket Close Function (Closes a Local Socket)
SOCKET socket_close(const SOCKET socket);

//Socket Check Read Function (Checks How Many Bytes there are to be Read, -1 on Error)
int socket_check_read(const SOCKET socket,const unsigned int time_out=0);

//Socket Peek Function (Same as socket_read but Leaves Bytes in Socket Buffer)
int socket_peek(const SOCKET socket,void* buffer,const unsigned int size,const int flags=0);

//Socket Read Function (Reads Bytes from Socket Buffer)
int socket_read(const SOCKET socket,void* buffer,const unsigned int size,const int flags=0);

//Socket Write Function (Writes Bytes to Socket)
int socket_write(const SOCKET socket,void* buffer,const unsigned int size,const int flags=0);

//End Define Guards
#endif

//Example (You need to make a folder called web and put index.html and not_found.html, located in comments below this example, in it for this to work)
/*
//Basic Web Server Source
//	Created By:		Mike Moss
//	Modified On:	03/12/2013

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

//Service Client Function Declaration
void service_client(msl::socket& client,const std::string& message);

//Main
int main()
{
	//Create Server
	msl::socket server("0.0.0.0:8080");
	server.create_tcp();

	//Check Server
	if(server)
		std::cout<<"Server started =)"<<std::endl;
	else
		std::cout<<"Server failed =("<<std::endl;

	//Vectors for Clients
	std::vector<msl::socket> clients;
	std::vector<std::string> client_messages;

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

	//Call Me Plz T_T
	return 0;
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

		//File Data Variable
		std::string file;

		//Load File
		if(msl::file_to_string(web_root+"/"+request,file))
			client<<msl::http_pack_string(file,mime_type);

		//Bad File
		else if(msl::file_to_string(web_root+"/not_found.html",file))
			client<<msl::http_pack_string(file);

		//not_found.html...Not Found?!  >_<
		else
			client.close();
	}

	//Other Requests (Just kill connection...it's either hackers or idiots...)
	else
	{
		client.close();
	}
}
*/

//index.html
/*
<html>
	<head>
		<title>Your here!</title>
	</head>
	<body>
		<center>Now go away...</center>
	</body>
</html>
*/

//not_found.html
/*
<html>
	<head>
		<title>Not found!</title>
	</head>
	<body>
		<center>T_T</center>
	</body>
</html>
*/