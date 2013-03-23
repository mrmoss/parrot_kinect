#!/bin/bash

g++ \
    kinect.cpp \
	raw_to_jpeg.cpp \
	libfree/cyber_kinect.cpp \
    msl/sprite.cpp \
    msl/socket.cpp \
    msl/file_util.cpp \
    msl/socket_util.cpp \
    msl/string_util.cpp \
	-pthread \
    -ljpeg \
    -lz \
    -lavcodec \
    -lswscale \
    -lavutil \
    -lfreenect \
    -lGLEW \
    -lglut \
	-lSOIL \
    -Wall -o kinect_run 

