#ifndef PDCONTROLLER_H_
#define PDCONTROLLER_H_

#include "kinect/Kinect.hpp"
#include "kinect/libfree/vec4.h"
#include "falconer.hpp"

class PDController
{
public:
	PDController(vec3 desired = vec3(0,0,1), vec3 gain_p = vec3(.2,.2,.2), vec3 gain_d = vec3(-5,10,-5));

	void set_desired_location(vec3 new_location);

	void autonomous_flight( ardrone & drone, Kinect & kinect);


private:
	Kinect* _kinect;
	ardrone* _drone;

	vec3 _desired_location;

	vec3 _error_old;
	//double x_error_old=0;
	//double y_error_old=0;
	//double z_error_old=0;

	vec3 _gain_p;
	//double x_gain_p=1;
	//double y_gain_p=1;
	//double z_gain_p=1;

	vec3 _gain_d;
	//double x_gain_d=-50;
	//double y_gain_d=10;
	//double z_gain_d=-50;
};

#endif //PDCONTROLLER_H_
