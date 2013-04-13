//Code based on Helisimple2 ... insert full documentation!!!

#include <stdlib.h>
#include "helisimple/raw_to_jpeg.h"
#include <pthread.h>

//Kinect libraries
#include "libfree/cyber_kinect.h"

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

double x_size = 3;
double y_size = 1;
double z_size = 3;
int main(int argc,char* argv[])
{
    bool doOpenGL=true;
    startKinectThread(argc, argv, x_size, y_size, z_size, z_size/2+1.5, doOpenGL);

    while (true) {sleep(10);};
    return 0;
}

