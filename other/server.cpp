

std::string make_json()
{
	std::stringstream json;

	vec3 curr_location = getLocation();

	json << "{ \n";
	json << "current_x : " << curr_location.x << ",\n";
	json << "current_y : " << curr_location.y << ",\n";
	json << "current_z : " << curr_location.z << "\n";
	json << "}";

	return json.str();
}

void server_startup()
{
	pthread_create(&server_thread,NULL,server_thread_function,NULL);
}

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

		//Check for Index
		if(request=="")
			request="cyberAlaskaInterface.html";

		//Mime Type Variable (Default plain text)
		std::string mime_type="text/plain";

		//File Data Variable
		std::string file;

		bool loaded = false;

		else if(msl::ends_with(request,".jpg")||msl::ends_with(request,".jpeg"))
		{
			int pos=request.find('?');

			if(pos!=-1)
				request=request.substr(0,pos);

			mime_type="image/jpeg";

			pthread_mutex_lock(&image_mutex);
			loaded=msl::file_to_string(web_root+"/"+request,file);
			pthread_mutex_unlock(&image_mutex);
		}

		std::stringstream parse_sstr;
		std::stringstream output_sstr;

		else if(msl::starts_with(request,"uav/0/goto"))
		{
			for(unsigned int i = 0; i < request.size() ; ++i)
			{
				if( request[i] == '&')
					request[i] = ' ';
			}

			parse_sstr << request;

			pthread_mutex_lock(&drone_location_mutex);

			parse_sstr.ignore(request.size(), '=');
			parse_sstr >> desired_loc.x;

			parse_sstr.ignore(request.size(), '=');
			parse_sstr >> desired_loc.y;

			parse_sstr.ignore(request.size(), '=');
			parse_sstr >> desired_loc.z;
			desired_loc.z += + z_size / 2;

			pthread_mutex_unlock(&drone_location_mutex);

			output_sstr << "location recieved";
		}
		else if(msl::starts_with(request, "uav/0/takeoff"))
		{
			pthread_mutex_lock(&drone_command_mutex);
			heli->takeoff();
			pthread_mutex_unlock(&drone_command_mutex);

			output_sstr << "takeoff recieved";
		}
		else if(msl::starts_with(request, "uav/0/land"))
		{
			pthread_mutex_lock(&drone_command_mutex);
			heli->land();
			pthread_mutex_unlock(&drone_command_mutex);

			output_sstr << "land recieved";
		}
		else if(msl::starts_with(request, "uav/0/videoframe"))
		{
			msl::file_to_string(web_root+"/"+request,file);
		}
		else if(msl::starts_with(request, "uav/0/status"))
		{
			output_sstr << make_json();
		}
		else
		{
			std::cout << request << std::endl;
		}

		file += output_sstr.str();

		client<<msl::http_pack_string(file,mime_type);

		//not_found.html Not Found?!
		else
		{
			client.close();
		}
	}

	//Other Requests (Just kill connection...)
	else
	{
		//std::cout << message << std::endl;
		client.close();
	}
}

void* server_thread_function(void*)
{
	//Create Server
	msl::socket server("0.0.0.0:80");
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
				//std::cout << "client closed" << clients.size() << std::endl;
				clients[ii].close();
				clients.erase(clients.begin()+ii);
				client_messages.erase(client_messages.begin()+ii);
				--ii;
			}
		}
	}

	return NULL;
}
