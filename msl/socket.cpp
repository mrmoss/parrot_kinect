//Socket Source
//	Created By:		Mike Moss
//	Modified On:	03/12/2013

//Required Libraries:
//	wsock32 (windows only)

//Definitions for "socket.hpp"
#include "socket.hpp"

//C String Header (For memcpy)
#include <cstring>

//Exception Header
#include <stdexcept>

//Signal Header
#include <signal.h>

//Time Header
#include <time.h>

//End Line Variable(std::endl doesn't work)
const char msl::endl='\n';

//IPv4 Address Class Constructor(Default)
msl::ipv4::ipv4(const unsigned char ip[4],const unsigned short port):_port(port)
{
	//If There's Data
	if(ip!=NULL)
	{
		_ip[0]=ip[0];
		_ip[1]=ip[1];
		_ip[2]=ip[2];
		_ip[3]=ip[3];
	}

	//If NULL is Passed
	else
	{
		_ip[0]=0;
		_ip[1]=0;
		_ip[2]=0;
		_ip[3]=0;
	}
}

//IPv4 Address Class Copy Constructor
msl::ipv4::ipv4(const msl::ipv4& copy)
{
	_ip[0]=copy._ip[0];
	_ip[1]=copy._ip[1];
	_ip[2]=copy._ip[2];
	_ip[3]=copy._ip[3];
	_port=copy._port;
}

//IPv4 Address Class Copy Assignment Operator
msl::ipv4& msl::ipv4::operator=(const msl::ipv4& copy)
{
	if(this!=&copy)
	{
		_ip[0]=copy._ip[0];
		_ip[1]=copy._ip[1];
		_ip[2]=copy._ip[2];
		_ip[3]=copy._ip[3];
		_port=copy._port;
	}

	return *this;
}

//IPv4 Address Class Build Function(Returns Raw Socket Address)
sockaddr_in msl::ipv4::build() const
{
	sockaddr_in ret;
	ret.sin_family=AF_INET;
	ret.sin_port=htons(_port);
	memcpy(&ret.sin_addr,_ip,4);

	return ret;
}

//IPv4 Address Class String Accessor(X.X.X.X:PORT)
std::string msl::ipv4::str() const
{
	std::ostringstream ostr;

	ostr<<static_cast<unsigned int>(_ip[0])<<'.'<<static_cast<unsigned int>(_ip[1])<<'.'
		<<static_cast<unsigned int>(_ip[2])<<'.'<<static_cast<unsigned int>(_ip[3])<<':'
		<<static_cast<unsigned int>(_port);

	return ostr.str();
}

//Socket Class Constructor(Default)
msl::socket::socket(const std::string& address):std::ostream(reinterpret_cast<std::streambuf*>(NULL)),_socket(SOCKET_ERROR),_hosting(false)
{
	//Parsing Variables
	unsigned char ip[4]={0,0,0,0};
	unsigned short port=0;
	std::istringstream istr(address,std::ios_base::in);

	//Find 4 IP Octets and a Port (5 Things Total)
	for(unsigned int ii=0;ii<5;++ii)
	{
		//Temporary Variables
		char remove_delimeter;
		unsigned int temp;

		//Bad Read
		if(!(istr>>temp))
			throw std::runtime_error("socket::socket - address is invalid!");

		//IP Address
		if(ii<4)
		{
			ip[ii]=temp;
		}

		//Port
		else
		{
			port=temp;
		}

		//Remove Delimeters (.'s and :'s)
		istr>>remove_delimeter;

		//Check For Bad Delimeters
		if((ii<3&&remove_delimeter!='.')||(ii>2&&remove_delimeter!=':'))
			throw std::runtime_error("socket::socket - delimeter is invalid!");
	}

	//Set Address
	_address=msl::ipv4(ip,port);
}

//Socket Class Copy Constructor
msl::socket::socket(const msl::socket& copy):std::ostream(reinterpret_cast<std::streambuf*>(NULL)),_address(copy._address),_socket(copy._socket),_hosting(copy._hosting)
{}

//Socket Class Copy Assignment Operator
msl::socket& msl::socket::operator=(const msl::socket& copy)
{
	if(this!=&copy)
	{
		_address=copy._address;
		_socket=copy._socket;
		_hosting=copy._hosting;
	}

	return *this;
}

//Boolean Operator (Tests If Socket Is Good)
msl::socket::operator bool() const
{
	//Check for Errored Socket
	if(_socket==static_cast<unsigned int>(SOCKET_ERROR)||_socket==static_cast<unsigned int>(INVALID_SOCKET))
		return false;

	//Check Reading Error
	if(check()<0)
		return false;

	//Check Client Errors
	if(!_hosting&&check()>0)
	{
		char temp;

		//Try Reading
		if(!socket_peek(_socket,&temp,1))
			return false;
	}

	//Else Socket is Good
	return true;
}

//Not Operator (For Boolean Operator)
bool msl::socket::operator!() const
{
	return !static_cast<bool>(*this);
}

//Create Function (Hosts a Socket Locally)
void msl::socket::create()
{
	_socket=socket_create(_address);
	_hosting=true;
}

//Connect Function (Connects to a Remote Socket)
void msl::socket::connect()
{
	_socket=socket_connect(_address);
	_hosting=false;
}

//Close Function (Closes a Local Socket)
void msl::socket::close()
{
	socket_close(_socket);
}

//Accept Function (Accepts a Remote Connection to a Local Socket)
msl::socket msl::socket::accept()
{
	msl::socket ret;

	if(check()>0)
		ret._socket=socket_accept(_socket,ret._address);

	return ret;
}

//Read Function (Returns True if Read was Successful)
bool msl::socket::read(void* buffer,const unsigned int size) const
{
	return socket_read(_socket,buffer,size);
}

//Write Function (Returns True if Write was Successful)
bool msl::socket::write(void* buffer,const unsigned int size) const
{
	return socket_write(_socket,buffer,size);
}

//Check Function (Checks How Many Bytes there are to be Read, -1 on Error)
int msl::socket::check() const
{
	return socket_check_read(_socket);
}

//Temporary Socket Variables
static bool socket_ignore_sigpipe=false;
static bool socket_inited=false;

//Unix Signal Setup
#if(!defined(_WIN32)||defined(__CYGWIN__))
	typedef void(*skt_signal_handler_fn)(const int sig);
	static skt_signal_handler_fn socket_fallback_sigpipe=NULL;

	static void socket_sigpipe_handler(const int sig)
	{
		if(socket_ignore_sigpipe)
		{
			signal(SIGPIPE,socket_sigpipe_handler);
		}
		else
		{
			socket_fallback_sigpipe(sig);
		}
	}
#endif

//Socket Initialize Function
static void socket_init()
{
	//If Not Initialized
	if(!socket_inited)
	{
		//Set Initialized
		socket_inited=true;

		//Windows Initialization
		#if(defined(_WIN32)&&!defined(__CYGWIN__))
			WSADATA temp;
			WSAStartup(0x0002,&temp);

		//Unix Initialize
		#else
			socket_fallback_sigpipe=signal(SIGPIPE,socket_sigpipe_handler);
		#endif
	}
}

//Socket Create Function (Hosts a Socket Locally)
SOCKET socket_create(const msl::ipv4 ip,const unsigned int time_out,const bool UDP,const unsigned int buffersize)
{
	//Initialize Sockets
	socket_init();

	//Connection Variables
	unsigned int time_start=time(0);
	sockaddr_in address=ip.build();
	socklen_t address_length=sizeof(address);
	int on=1;
	unsigned int type=SOCK_STREAM;
	SOCKET ret=socket(AF_INET,type,0);

	//UDP Connection Setup
	if(UDP)
		type=SOCK_DGRAM;

	//Try to Create Socket
	do
	{
		//Create Socket
		ret=socket(AF_INET,type,0);

		//Check for Errors
		if(ret!=static_cast<unsigned int>(SOCKET_ERROR))
		{
			if(setsockopt(ret,SOL_SOCKET,SO_REUSEADDR,reinterpret_cast<const char*>(&on),sizeof(int)))
				return socket_close(ret);

			if(UDP&&setsockopt(ret,SOL_SOCKET,SO_RCVBUF,reinterpret_cast<const char*>(&buffersize),address_length)==static_cast<int>(SOCKET_ERROR))
				return socket_close(ret);

			if(UDP&&setsockopt(ret,SOL_SOCKET,SO_SNDBUF,reinterpret_cast<const char*>(&buffersize),address_length)==static_cast<int>(SOCKET_ERROR))
				return socket_close(ret);

			if(bind(ret,(sockaddr*)&address,sizeof(address)))
				return socket_close(ret);

			if(!UDP&&listen(ret,5))
				return socket_close(ret);

			if(getsockname(ret,(sockaddr*)&address,&address_length))
				return socket_close(ret);

			return ret;
		}
	}
	while(time(0)-time_start<time_out);

	//Close on Error
	return socket_close(ret);
}

//Socket Connection Function (Connects to a Remote Socket)
SOCKET socket_connect(const msl::ipv4 ip,const unsigned int time_out,const bool UDP)
{
	//Initialize Sockets
	socket_init();

	//Connection Variables
	unsigned int time_start=time(0);
	sockaddr_in address=ip.build();
	int type=SOCK_STREAM;
	SOCKET ret=SOCKET_ERROR;

	//UDP Connection Setup
	if(UDP)
		type=SOCK_DGRAM;

	//Try to Create Socket
	do
	{
		//Create Socket
		ret=socket(AF_INET,type,0);

		//Connect and Check for Good Socket
		if(connect(ret,reinterpret_cast<sockaddr*>(&address),sizeof(address))!=SOCKET_ERROR)
			return ret;
	}
	while(time(0)-time_start<time_out);

	//Return Error Otherwise
	return SOCKET_ERROR;
}

//Socket Accept Function (Accepts a Remote Connection to a Local Socket)
SOCKET socket_accept(const SOCKET socket,msl::ipv4& client_ip,const unsigned int time_out)
{
	//Check for Bad Host
	if(socket==static_cast<unsigned int>(SOCKET_ERROR))
		return false;

	//Initialize Sockets
	socket_init();

	//Connection Variables
	unsigned int time_start=time(0);
	sockaddr_in address;
	socklen_t address_length=sizeof(address);
	SOCKET ret=SOCKET_ERROR;

	//Try to Create Socket
	do
	{
		//Create Socket
		ret=accept(socket,(sockaddr*)&address,&address_length);

		//Check for Good Socket
		if(ret!=static_cast<unsigned int>(SOCKET_ERROR)&&ret!=static_cast<unsigned int>(INVALID_SOCKET))
		{
			client_ip=msl::ipv4(reinterpret_cast<unsigned char*>(&address.sin_addr),ntohs(address.sin_port));
			return ret;
		}
	}
	while(time(0)-time_start<time_out);

	//Return Error Otherwise
	return SOCKET_ERROR;
}

//Socket Close Function (Closes a Local Socket)
SOCKET socket_close(const SOCKET socket)
{
	//If Good Socket
	if(socket!=static_cast<unsigned int>(SOCKET_ERROR))
	{
		//Initialize Sockets
		socket_init();

		//Windows Close Socket
		#if(defined(_WIN32)&&!defined(__CYGWIN__))
			closesocket(socket);
		#else

		//Unix Close Socket
			socket_ignore_sigpipe=true;
			close(socket);
			socket_ignore_sigpipe=false;
		#endif
	}

	//Return Error
	return SOCKET_ERROR;
}

//Socket Check Read Function (Checks How Many Bytes there are to be Read, -1 on Error)
int socket_check_read(const SOCKET socket,const unsigned int time_out)
{
	//Check for Bad Socket
	if(socket==static_cast<unsigned int>(SOCKET_ERROR))
		return -1;

	//Initialize Sockets
	socket_init();

	//Reading Variables
	unsigned int time_start=time(0);
	timeval temp={0,0};
	fd_set rfds;
	FD_ZERO(&rfds);
	FD_SET(socket,&rfds);

	//Try to Read from Socket
	do
	{
		//Get Byte Number in Read Buffer
		socket_ignore_sigpipe=true;
		unsigned int read=select(1+socket,&rfds,NULL,NULL,&temp);
		socket_ignore_sigpipe=false;
		return read;
	}
	while(time(0)-time_start<time_out);

	//Return -1 on Error
	return -1;
}

//Socket Peek Function (Same as socket_read but Leaves Bytes in Socket Buffer)
bool socket_peek(const SOCKET socket,void* buffer,const unsigned int size)
{
	//Check for Bad Socket
	if(socket==static_cast<unsigned int>(SOCKET_ERROR))
		return false;

	//Initialize Sockets
	socket_init();

	//Reading Variables
	unsigned int bytes_unread=size;

	//While Socket is Good and There are Bytes to Read
	while(bytes_unread>0&&socket!=static_cast<unsigned int>(SOCKET_ERROR))
	{
		//Get Bytes in Read Buffer
		socket_ignore_sigpipe=true;
		unsigned int bytes_read=recv(socket,reinterpret_cast<char*>(buffer)+(size-bytes_unread),bytes_unread,MSG_PEEK);
		socket_ignore_sigpipe=false;

		//On Error
		if(bytes_read<=0)
			return false;

		//Subtract Read Bytes
		bytes_unread-=bytes_read;
	}

	//Return Success
	return true;
}

//Socket Read Function (Reads Bytes from Socket Buffer)
bool socket_read(const SOCKET socket,void* buffer,const unsigned int size)
{
	//Check for Bad Socket
	if(socket==static_cast<unsigned int>(SOCKET_ERROR))
		return false;

	//Initialize Sockets
	socket_init();

	//Reading Variables
	unsigned int bytes_unread=size;

	//While Socket is Good and There are Bytes to Read
	while(bytes_unread>0&&socket!=static_cast<unsigned int>(SOCKET_ERROR))
	{
		//Get Bytes in Read Buffer
		socket_ignore_sigpipe=true;
		unsigned int bytes_read=recv(socket,reinterpret_cast<char*>(buffer)+(size-bytes_unread),bytes_unread,0);
		socket_ignore_sigpipe=false;

		//On Error
		if(bytes_read<=0)
			return false;

		//Subtract Read Bytes
		bytes_unread-=bytes_read;
	}

	//Return Success
	return true;
}

//Socket Write Function (Writes Bytes to Socket)
bool socket_write(const SOCKET socket,void* buffer,const unsigned int size)
{
	//Check for Bad Socket
	if(socket==static_cast<unsigned int>(SOCKET_ERROR))
		return false;

	//Initialize Sockets
	socket_init();

	//Writing Variables
	unsigned int bytes_unsent=size;

	//While Socket is Good and There are Bytes to Write
	while(bytes_unsent>0&&socket!=static_cast<unsigned int>(SOCKET_ERROR))
	{
		//Send Bytes into Write Buffer
		socket_ignore_sigpipe=true;
		int bytes_sent=send(socket,reinterpret_cast<char*>(buffer)+(size-bytes_unsent),bytes_unsent,0);
		socket_ignore_sigpipe=false;

		//On Error
		if(bytes_sent<=0)
			return false;

		//Subtract Written Bytes
		bytes_unsent-=(unsigned int)bytes_sent;
	}

	//Return Success
	return true;
}