#include "PIDController.hpp"
#include <iostream>

PIDController::PIDController(vec3 desired, vec3 gain_p , vec3 gain_d, vec3 gain_i):
		_desired_location(desired), _gain_p(gain_p), _gain_d(gain_d), _gain_i(gain_i)
{
	_error_sum = vec3(0,0,0);
}

template <typename T>
void clamp(T& input,const T min,const T max)
{
	if(input<min)
		input=min;

	if(input>max)
		input=max;
}

void PIDController::set_desired_location(vec3 new_location)
{
	_desired_location = new_location;
}

void PIDController::autonomous_flight(ardrone & drone, Kinect & kinect)
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

		_error_sum.x += x_error_new;
		_error_sum.y += y_error_new;
		_error_sum.z += z_error_new;

		roll = ( _gain_p.x * x_error_new + _gain_d.x * ( _error_old.x - x_error_new)  + _gain_i.x * _error_sum.x);
		pitch = - ( _gain_p.z * z_error_new + _gain_d.z * ( _error_old.z - z_error_new) + _gain_i.z * _error_sum.z);

		_error_old.x = x_error_new;
		_error_old.y = y_error_new;
		_error_old.z = z_error_new;

		double max=1;
		clamp(roll,-max,max);
		clamp(pitch,-max,max);
	}

	drone.manuever(altitude,pitch,roll,yaw);

	//std::cout << "PID :" << pitch << " " << roll << std::endl;
}
