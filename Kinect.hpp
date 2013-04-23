#ifndef KINECT_H_
#define KINECT_H_

#include "libfree/cyber_kinect.h"

class Kinect
{
public:
	Kinect(double x_field_size = 3, double y_field_size = 1, double z_field_size = 2, double distance_from_kinect = 1.5, bool openGL_on = false);

	int start_thread();

	vec3 update_location();

	vec3 get_location();

	vec3 _location;

	double _x_field_size;
	double _y_field_size;
	double _z_field_size;
	double _distance_from_kinect;

	bool _openGL_on;
};

#endif //KIENCT_H_
