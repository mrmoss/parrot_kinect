//File Utility Header
#include "../msl/file_util.hpp"

//IO Stream Header
#include <iostream>

//Socket Header
#include "../msl/socket.hpp"

//Socket Utility Header
#include "../msl/socket_util.hpp"

//String Header
#include <string>

//String Utility Header
#include "../msl/string_util.hpp"

//Vector Header
#include <vector>

//Service Client Function Declaration - YOU NEED TO LOOK AT THIS DEFINITION
void service_client(msl::socket& client,const std::string& message);

//Main
int main()
{
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
        
        int pos = request.find('?');
        if(pos != -1)
        {
            request = request.substr(0, pos);
        }
        
        std::cout << request << std::endl; 
        
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
		else if(msl::ends_with(request,".jpeg") || msl::ends_with(request,".jpg"))
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
        
		//not_found.html Not Found?!
		else
			client.close();
	}
    
	//Other Requests (Just kill connection...)
	else
	{
		client.close();
	}
}
