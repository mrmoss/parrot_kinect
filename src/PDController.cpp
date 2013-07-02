#include "PDController.hpp"
#include "kinect/coordinate_system.hpp"
#include <iostream>

PDController::PDController(vec3 desired, vec3 gain_p , vec3 gain_d ):
	_desired_location(desired), _gain_p(gain_p), _gain_d(gain_d)
{
	_desired_location.y = kcs_desired_height;
}

template <typename T>
void clamp(T& input,const T min,const T max)
{
	if(input<min)
		input=min;

	if(input>max)
		input=max;
}

void PDController::set_desired_location(vec3 new_location)
{
	_desired_location = new_location;
}

void PDController::autonomous_flight(ardrone & drone, Kinect & kinect)
{
	double roll = 0;
	double pitch = 0;
	double yaw = 0;
	double altitude = 0;

	if(kinect.get_location().z!=-1)
	{
		double x_error_new = kinect.get_location().x - _desired_location.x;
		double y_error_new = kinect.get_location().y - _desired_location.y;
		double z_error_new = kinect.get_location().z - _desired_location.z;

		roll = - ( _gain_p.x * x_error_new + _gain_d.x * ( _error_old.x - x_error_new) );
		pitch = ( _gain_p.z * z_error_new + _gain_d.z * ( _error_old.z - z_error_new ));
		altitude = - ( _gain_p.y * y_error_new + _gain_d.y * ( _error_old.y - y_error_new ));

		_error_old.x = x_error_new;
		_error_old.y = y_error_new;
		_error_old.z = z_error_new;
		
		//std::cout<<altitude<<"\t\t"<<y_error_new<<std::endl;

		double max=1;
		clamp(roll,-max,max);
		clamp(pitch,-max,max);
		clamp(altitude,-max,max);
	}

	drone.manuever(altitude,pitch,roll,yaw);
	//std::cout << "PD :" << pitch << " " << roll << std::endl;
}
