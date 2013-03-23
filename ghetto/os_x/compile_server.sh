#!/bin/bash

clang++ server/server.cpp \
        msl/sprite.cpp \
        msl/socket.cpp \
        msl/file_util.cpp \
        msl/socket_util.cpp \
        msl/string_util.cpp \
        /usr/include/SOIL/SOIL.c \
        /usr/include/SOIL/stb_image_aug.c \
        -framework OpenGL \
        -framework Glut \
        -lGLEW \
        -o server_run
