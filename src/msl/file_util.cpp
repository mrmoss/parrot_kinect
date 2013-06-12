//File Utility Source
//	Created By:		Mike Moss
//	Modified On:	03/12/2013

//Definitions for "file_util.hpp"
#include "file_util.hpp"

//File Stream Header
#include <fstream>

//File Status Header
#include <sys/stat.h>

//Windows Defines
#if defined(_WIN32)&&!defined(__CYGWIN__)
	#include <windows.h>
	#define LS "dir /b "
	#define RM "del /q "
	#define RMD "rd /s /b "

//Unix Defines
#else
	#include <stdlib.h>
	#define LS "ls "
	#define RM "rm -f "
	#define RMD "rm -rf "
#endif

//List Directory Function (Lists files in directory as strings in a vector)
std::vector<std::string> msl::list_directory(const std::string& directory)
{
	//Temp Variables
	std::vector<std::string> files;
	std::string temp="";

	//Get File Listing
	if(system((LS" "+directory+" > ._______msl_file_listing_______132584376582734651834.msl").c_str())==-1)
		return files;

	//Create Filestream
	std::ifstream istr("._______msl_file_listing_______132584376582734651834.msl");

	//Get Files
	while(istr>>temp)
		files.push_back(temp);

	//Close Filestream
	istr.close();

	//Delete Files
	if(system(RM" ._______msl_file_listing_______132584376582734651834.msl")==-1)
	{}

	//Return Vector
	return files;
}

//File to String Function (Loads a file from disk as a string, returns false on error)
bool msl::file_to_string(const std::string& filename,std::string& data,const bool binary)
{
	//Read In Variable
	char buffer;

	//Open Flag
	std::ios_base::openmode flags=std::ios_base::in;

	//Binary Flag
	if(binary)
		flags|=std::ios_base::binary;

	//Open File
	std::ifstream istr(filename.c_str(),flags);

	//Unset Skip White Space Flag
	istr.unsetf(std::ios_base::skipws);

	//Test Stream
	if(!istr)
		return false;

	//Clear Data
	data="";

	//Read File into Buffer
	while(istr>>buffer)
		data+=buffer;

	//Close File
	istr.close();

	//Return Good
	return true;
}

//String to File Function (Saves a string to disk as a file, returns false on error)
bool msl::string_to_file(const std::string& data,const std::string& filename,const bool binary)
{
	//Saved Variable
	bool saved=false;

	//Open Flag
	std::ios_base::openmode flags=std::ios_base::out;

	//Binary Flag
	if(binary)
		flags|=std::ios_base::binary;

	//Open File
	std::ofstream ostr(filename.c_str(),flags);

	//Write New File
	saved=(ostr<<data);

	//Close File
	ostr.close();

	//Return Saved
	return saved;
}

//Remove File Function (Removes a file from disk, returns false on error)
bool msl::remove_file(const std::string& filename)
{
	return (system((RM+filename).c_str())!=-1);
}

//Remove Directory Function (Removes a directory from disk recursively, returns false on error)
bool msl::remove_directory(const std::string& directory)
{
	return (system((RMD+directory).c_str())!=-1);
}

//File Exists Function (Checks if a file exists on disk)
bool msl::file_exists(const std::string& filename)
{
	struct stat buf;
	return (stat(filename.c_str(),&buf)!=-1);
}