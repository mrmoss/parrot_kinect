/*
 * cyber_kinect.h
 *
 *  Created on: Jan 8, 2013
 *      Author: shaun
 */

#ifndef CYBER_KINECT_H_
#define CYBER_KINECT_H_

#include "vec4.h"

// Pass in argc and **argv from main.
// Will create a bounding box of size x, y, z meters.
// Box will be centered at dist meters from the kinect.
int startKinectThread(int argc, char **argv, float x, float y, float z, float dist, bool using_open_gl = true);

// Returns the x, y, z location of the UAV in meters with kinect at origin.
vec3 getLocation();


#endif /* CYBER_KINECT_H_ */
