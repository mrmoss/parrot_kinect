#ifndef KINECT_COORDINATE_SYSTEM
#define KINECT_COORDINATE_SYSTE

//desired hieght with 0 being level with kinect
#define kcs_desired_height 0

//Size of viewable field (full size of field, not 1/2 or 1/4 or distance+1/2 of...)
#define kcs_x_field_size 1.5
#define kcs_y_field_size 1
#define kcs_z_field_size 3

//Distance FROM kinect
#define kcs_distance_from_kinect 2

//Distance TO origin (center of field)
#define kcs_distance_to_origin (kcs_z_field_size/2+kcs_distance_from_kinect)

#endif
