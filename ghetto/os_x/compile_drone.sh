#!/bin/bash

clang++ $1 \
	raw_to_jpeg.cpp \
	SDL/SDLmain.m \
	helisimple/CDecoder.cpp \
	helisimple/CHeli.cpp \
	helisimple/CRecognition.cpp \
	helisimple/CGui.cpp \
	helisimple/CImageClient.cpp \
	helisimple/CRawImage.cpp \
	helisimple/CThread.cpp \
	helisimple/app.cpp \
	helisimple/default.cpp \
	helisimple/navdata.cpp \
	helisimple/at_cmds.cpp \
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
	-framework SDL \
    -framework Cocoa \
    -w -o $2