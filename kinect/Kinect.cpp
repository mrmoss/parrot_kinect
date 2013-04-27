#include "Kinect.hpp"
#include <cstdlib>

Kinect::Kinect(double x_field_size, double y_field_size, double z_field_size, double distance_from_kinect, bool openGL_on ):
	_x_field_size(x_field_size), _y_field_size(y_field_size), _z_field_size(z_field_size), _distance_from_kinect(distance_from_kinect), _openGL_on(openGL_on), _location(0,0,-1)
{

}

int Kinect::start_thread()
{
	startKinectThread(0, NULL, _x_field_size, _y_field_size, _z_field_size, _z_field_size/2.0 + _distance_from_kinect, _openGL_on);
}

vec3 Kinect::update_location()
{
	_location = getLocation();
	return _location;
}

vec3 Kinect::get_location()
{
	return _location;
}
