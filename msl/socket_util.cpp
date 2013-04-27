//Socket Utility Source
//	Created By:		Mike Moss
//	Modified On:	03/12/2013

//Definitions for "socket_util.hpp"
#include "socket_util.hpp"

//String Stream Header
#include <sstream>

//HTTP Create Header Function
std::string msl::http_create_header(const unsigned int message_size,const std::string& mime_type,const bool compressed)
{
	//Create HTML Header
	std::ostringstream header;
	header<<"HTTP/1.1 200 OK\n";
	header<<"Server: Super Lightning Automatic Systematic Wisdomatic Server 3000 v6.54.33.2.1a-177b-c Stable Beta\n";
	header<<"Last-Modified: From the future\n";
	header<<"Content-Length: "<<message_size<<'\n';
	header<<"Content-Type: "<<mime_type<<"; charset=UTF-8\n";

	//Support Compressed Files
	if(mime_type=="application.x-gzip"||compressed)
		header<<"Content-Encoding: gzip\n";

	//Set Connection Type
	header<<"Connection: close\n\n";

	//Return HTML Header
	return header.str();
}

//HTTP to ASCII Function (Converts a string with http symbols to ascii symbols)
std::string msl::http_to_ascii(std::string symbols)
{
	//Replace Newlines
	while(true)
	{
		//Find a Newline
		int pos=symbols.find("%0D%0A");

		//If No Newlines Break
		if(pos==-1)
			break;

		//Replace Newlines
		symbols.replace(pos,6,"\n");
	}


	//Parse String
	for(unsigned int ii=0;ii<symbols.size();++ii)
	{
		//Found a Symbol
		if(symbols[ii]=='%')
		{
			//Check for Invalid Symbols
			if(ii+2<symbols.size()&&(isdigit(symbols[ii+1])||(symbols[ii+1]>='A'&&symbols[ii+1]<='F'))
				&&(isdigit(symbols[ii+2])||(symbols[ii+2]>='A'&&symbols[ii+2]<='F')))
			{
				//Temp Character
				char symbol=0;

				//Get 1's Digit
				if(symbols[ii+2]>'9')
					symbol+=symbols[ii+2]-65+10;
				else
					symbol+=symbols[ii+2]-48;

				//Get 16's Digit
				if(symbols[ii+1]>'9')
					symbol+=(symbols[ii+1]-65+10)*16;
				else
					symbol+=(symbols[ii+1]-48)*16;

				//Insert Symbol
				symbols[ii]=symbol;

				//Remove Extra Space
				symbols.erase(ii+1,2);
			}
		}
	}

	//Return Converted String
	return symbols;
}

//HTTP Pack String Function (Packages a string for http travel)
std::string msl::http_pack_string(const std::string& message,const std::string& mime_type,const bool compressed)
{
	return msl::http_create_header(message.size(),mime_type,compressed)+message;
}