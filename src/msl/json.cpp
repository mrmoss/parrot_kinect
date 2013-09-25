//JSON Source
//	Created By:		Mike Moss
//	Modified On:	05/26/2013

//Definitions for "json.hpp"
#include "json.hpp"

//Stream Out Operator
std::ostream& operator<<(std::ostream& lhs,const msl::json& rhs)
{
	return (lhs<<rhs.str());
}

//Constructor (Default)
msl::json::json(const std::string& json_string)
{
	//States Enum for State Machine
	enum state
	{
		bracket,
		quote_a,
		variable,
		colon,
		quote_b,
		value,
		comma,
		done,
		error
	};

	//Parsing Variables
	state parse_state=bracket;
	std::string parse_variable="";
	std::string parse_value="";
	bool skip_whitespace=true;

	//Go Through JSON Sting
	for(unsigned int ii=0;ii<json_string.size();++ii)
	{
		//Skip Whitespace
		if(skip_whitespace)
		{
			while(ii<json_string.size()&&(json_string[ii]==' '||json_string[ii]=='\t'||json_string[ii]=='\r'||json_string[ii]=='\n'))
				++ii;

			skip_whitespace=false;
		}

		//Look For Opening Bracket
		if(parse_state==bracket)
		{
			//Default Error
			parse_state=error;

			//Found, Look For Opening Quote (Variable)
			if(json_string[ii]=='{')
			{
				parse_state=quote_a;
				skip_whitespace=true;
			}
		}

		//Look For Opening Quote (Variable)
		else if(parse_state==quote_a)
		{
			//Default Error
			parse_state=error;

			//Found, Look For Variable
			if(json_string[ii]=='\"')
			{
				parse_state=variable;
				parse_variable="";
			}
		}


		//Look For Variable
		else if(parse_state==variable)
		{
			//Found End Quote Without a Trailing Slash, Look For Colon
			if(json_string[ii]=='\"'&&ii>0&&json_string[ii-1]!='\\')
			{
				parse_state=colon;
				skip_whitespace=true;
			}

			//Else Add to Variable
			else
			{
				parse_variable+=json_string[ii];
			}
		}

		//Look For Colon
		else if(parse_state==colon)
		{
			//Default Error
			parse_state=error;

			//Found Colon, Look For Opening Quote (Value)
			if(json_string[ii]==':')
			{
				parse_state=quote_b;
				skip_whitespace=true;
			}
		}

		//Look For Opening Quote (Variable)
		else if(parse_state==quote_b)
		{
			//Default Error
			parse_state=error;

			//Found, Look For Value
			if(json_string[ii]=='\"')
			{
				parse_state=value;
				parse_value="";
			}
		}

		//Look For Variable
		else if(parse_state==value)
		{
			//Found End Quote Without a Trailing Slash, Look For Comma
			if(json_string[ii]=='\"'&&ii>0&&json_string[ii-1]!='\\')
			{
				set(parse_variable,parse_value);
				parse_state=comma;
				skip_whitespace=true;
			}

			//Else Add to Value
			else
			{
				parse_value+=json_string[ii];
			}
		}

		//Look For Comma
		else if(parse_state==comma)
		{
			//Default Error
			parse_state=error;

			//Found Comma, Look For Opening Quote (Variable)
			if(json_string[ii]==',')
			{
				parse_state=quote_a;
				skip_whitespace=true;
			}

			//Else Found Closing Bracket, Exit
			else if(json_string[ii]=='}')
			{
				parse_state=done;
				break;
			}
		}

		//Found Error, Break
		else
		{
			break;
		}
	}

	//If Error, Clear Variables
	if(parse_state!=done)
		_data.clear();
}

//Size Accessor (Returns number of variables)
unsigned int msl::json::size() const
{
	return _data.size();
}

//Get Operator (Returns variable from an index)
std::string msl::json::get(const unsigned int index)
{
	//Create a Count Variable (Damn iterators...)
	unsigned int count=0;

	//Iterate Through Variables
	for(std::map<std::string,std::string>::const_iterator ii=_data.begin();ii!=_data.end();++ii)
	{
		//Found Variable, Return Value
		if(count==index)
			return ii->first;

		//Increment Counter
		++count;
	}

	//Return Empty Value When Not Found
	return "";
}

//Get Operator (Returns the value of a variable)
std::string msl::json::get(const std::string& index)
{
	return _data[index];
}

//String Function (Returns the JSON string)
std::string msl::json::str() const
{
	//Add Initial Bracket
	std::string json_string="{";

	//Go Through Variables
	for(std::map<std::string,std::string>::const_iterator ii=_data.begin();ii!=_data.end();++ii)
	{
		//Add Variable to String
		json_string+="\""+ii->first+"\":";

		//Check for JSON Object Value
		if(msl::starts_with(ii->second,"{")&&msl::ends_with(ii->second,"}"))
			json_string+=ii->second;

		//Other Data Value
		else
			json_string+="\""+ii->second+"\"";

		//Get Next Variable
		std::map<std::string,std::string>::const_iterator next=ii;
		++next;

		//If Next is Not Last Add Comma
		if(next!=_data.end())
			json_string+=",";
	}

	//Add Final Bracket
	json_string+="}";

	//Return JSON String
	return json_string;
}