#include "PDController.hpp"

	PDController::PDController(Kinect & kinect, ardrone & drone, vec3 desired, vec3 gain_p , vec3 gain_d ):
		_desired_location(desired), _gain_p(gain_p), _gain_d(gain_d)
	{
		_kinect = kinect;

		_drone = drone;
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

void PDController::autonomous_flight(float altitude, float pitch, float roll, float yaw)
{
	if(_kinect.get_location().z!=-1)
	{
		double x_error_new = _kinect.get_location().x - _desired_location.x;
		double y_error_new = _kinect.get_location().y - _desired_location.y;
		double z_error_new = _kinect.get_location().z - _desired_location.z;

		roll = ( _gain_p.x * x_error_new + _gain_d.x * ( _error_old.x - x_error_new) );
		pitch = - ( _gain_p.z * z_error_new + _gain_d.z * ( _error_old.z - z_error_new ));

		_error_old.x = x_error_new;
		_error_old.y = y_error_new;
		_error_old.z = z_error_new;

		double max=1;
		clamp(roll,-max,max);
		clamp(pitch,-max,max);
	}

	_drone.manuever(altitude,pitch,roll,yaw);
}
