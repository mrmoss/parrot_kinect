#ifndef KINECT_COORDINATE_SYSTEM
#define KINECT_COORDINATE_SYSTEM

//ALL MEASUREMENTS ARE IN METERS

//desired height with 0 being level with kinect
#define kcs_desired_height 0.0

//offset from kinect to the center of box
#define kcs_kinect_offset vec3(0.3,-0.3,0.0)

//Size of viewable field (full size of field, not 1/2 or 1/4 or distance+1/2 of...)
#define kcs_x_field_size 3.1
#define kcs_y_field_size 1
#define kcs_z_field_size 3
#define kcs_buffer 0.3

//Distance FROM kinect to edge of field
#define kcs_distance_from_kinect 3.0

//Distance TO origin (center of field)
#define kcs_distance_to_origin (kcs_z_field_size/2.0+kcs_distance_from_kinect)

#endif
