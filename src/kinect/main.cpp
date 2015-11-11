//Code based on Helisimple2 ... insert full documentation!!!
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#ifdef __unix__
#  include <unistd.h>
#endif

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
	{
		vec3 v=getLocation();
		printf("XYZ %5.2f %5.2f %5.2f \n",v.x,v.y,v.z);
#ifdef __unix__
		sleep(0);
#endif
	}

	return 0;
}
