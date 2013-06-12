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

#include "coordinate_system.hpp"

int main(int argc,char* argv[])
{
	startKinectThread(argc, argv, kcs_x_field_size, kcs_y_field_size, kcs_z_field_size, kcs_distance_to_origin);

	while (true)
	{}

	return 0;
}
