//String Utility Source
//	Created By:		Mike Moss
//	Modified On:	03/12/2013

//Definitions for "string_util.hpp"
#include "string_util.hpp"

//Convert To Boolean Function
bool msl::to_bool(std::string value)
{
	for(unsigned int ii=0;ii<value.size();++ii)
		if(value[ii]>='A'&&value[ii]<='Z')
			value[ii]+=32;

	return !(value=="false"||(value.size()==1&&value=="0"));
}

//Convert To Double Function
double msl::to_double(const std::string& value)
{
	//Convert Variables
	std::istringstream istr(value,std::ios_base::in);
	double temp=0.0;

	//Convert
	istr>>temp;

	//Return Converted Value
	return temp;
}

//Convert To Int Function
int msl::to_int(const std::string& value)
{
	//Convert Variables
	std::istringstream istr(value,std::ios_base::in);
	int temp=0;

	//Convert
	istr>>temp;

	//Return Converted Value
	return temp;
}

//Convert To Char Function
char msl::to_char(const std::string& value)
{
	//Integers Will Work!
	return to_int(value);
}

//Convert To Unsigned Char Function
unsigned char msl::to_uchar(const std::string& value)
{
	//Integers Will Work!
	int temp=to_int(value);

	//Unsigned Conversion Factor
	while(temp<0)
		temp+=256;

	//Return Converted Value
	return temp;
}

//Starts With Function (Checks if string starts with another string)
bool msl::starts_with(const std::string& str,const std::string& start)
{
	//Check if Sizes are Comparable
	if(start.size()<=str.size())
	{
		//Check Each Character
		for(unsigned int ii=0;ii<start.size();++ii)
			if(str[ii]!=start[ii])
				return false;

		//If Match
		return true;
	}

	//Default False
	return false;
}

//Ends With Function (Checks if string ends with another string)
bool msl::ends_with(const std::string& str,const std::string& end)
{
	//Check if Sizes are Comparable
	if(end.size()<=str.size())
	{
		//Check Each Character
		for(unsigned int ii=1;ii<=end.size();++ii)
			if(str[str.size()-ii]!=end[end.size()-ii])
				return false;

		//If Match
		return true;
	}

	//Default False
	return false;
}