//Code based on Helisimple2 ... insert full documentation!!!

#include <stdlib.h>
#include <pthread.h>

//Kinect libraries
#include "libfree/cyber_kinect.h"

//IO Stream Header
#include <iostream>

//String Header
#include <string>

//Vector Header
#include <vector>

double x_size = 2.5;
double y_size = 1;
double z_size = 3;
int main(int argc,char* argv[])
{
    startKinectThread(argc, argv, x_size, y_size, z_size, z_size/2+1.5);

    while (true)
    {}

    return 0;
}
