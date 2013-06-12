#include "Kinect.hpp"
#include <cstdlib>
#include "coordinate_system.hpp"

Kinect::Kinect(bool openGL_on):_location(0,0,-1), _x_field_size(kcs_x_field_size), _y_field_size(kcs_y_field_size), _z_field_size(kcs_z_field_size),
	_distance_from_kinect(kcs_distance_from_kinect), _openGL_on(openGL_on)
{

}

int Kinect::start_thread()
{
	return startKinectThread(0, NULL, _x_field_size, _y_field_size, _z_field_size, kcs_distance_to_origin, _openGL_on);
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
