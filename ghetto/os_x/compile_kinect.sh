#!/bin/bash

clang++ \
    kinect.cpp \
	raw_to_jpeg.cpp \
	libfree/cyber_kinect.cpp \
    msl/sprite.cpp \
    msl/socket.cpp \
    msl/file_util.cpp \
    msl/socket_util.cpp \
    msl/string_util.cpp \
    /usr/include/SOIL/SOIL.c \
    /usr/include/SOIL/stb_image_aug.c \
	-pthread \
    -ljpeg \
    -lz \
    -lavcodec \
    -lswscale \
    -lavutil \
    -lfreenect \
    -lGLEW \
    -framework glut \
	-framework openGL \
    -framework Cocoa \
    -w -o kinect_run 