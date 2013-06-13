@echo off
REM COMPILER VARIABLES
	SET OPTS=-O
	SET COMPILER=g++
	SET SRC_SOIL=../include/SOIL/SOIL.c ../include/SOIL/stb_image_aug.c
	SET SRC_MSL=%SRC_SOIL% ../../src/msl/2d.cpp ../../src/msl/2d_util.cpp ../../src/msl/file_util.cpp ../../src/msl/font.cpp ../../src/msl/glut_input.cpp ../../src/msl/socket.cpp ../../src/msl/socket_util.cpp ../../src/msl/sprite.cpp ../../src/msl/string_util.cpp ../../src/msl/time_util.cpp
	SET SRC_PARROT=%SRC_MSL% ../../src/main.cpp ../../src/falconer.cpp ../../src/raw_to_jpeg.cpp ../../src/kinect/libfree/*.cpp ../../src/kinect/Kinect.cpp ../../src/PDController.cpp ../../src/PIDController.cpp
	SET CFLAGS_PARROT=-Wall -Wno-unknown-pragmas -lpthread -std=c++11 -lm %OPTS%
	SET LIB_PARROT=-lopengl32 -lglu32 -lfreeglut -lglew32 -lavcodec -lswscale -ljpeg -lwsock32
	SET SRC_KINECT=../../src/kinect/main.cpp ../../src/kinect/Kinect.cpp ../../src/kinect/libfree/*.cpp
	SET CFLAGS_KINECT=-Wall %OPTS%
	SET LIB_KINECT=-lopengl32 -lglu32 -lfreeglut -lglew32

REM GO TO COMPILER DIRECTORY
	CD g++/bin

REM BUILD VARIABLES
	SET BUILD_PARROT=0
	SET BUILD_KINECT=0
	SET BUILD_CLEAN=0

REM CHECK COMMAND LINE OPTIONS
	SET ARGC=0
	FOR %%i IN (%*) do (
		SET /a ARGC+=1
		IF %%i==all SET BUILD_PARROT=1
		IF %%i==all SET BUILD_KINECT=1
		IF %%i==parrot SET BUILD_PARROT=1
		IF %%i==kinect SET BUILD_KINECT=1
		IF %%i==clean SET BUILD_CLEAN=1
	)

	IF %ARGC%==0 SET BUILD_PARROT=1
	IF %ARGC%==0 SET BUILD_KINECT=1

REM BUILD WHAT IS NEEDED
	IF %BUILD_PARROT%==1 %COMPILER% %CFLAGS_PARROT% %SRC_PARROT% -o ../../bin/parrot.exe %LIB_PARROT%
	IF %BUILD_KINECT%==1 %COMPILER% %CFLAGS_KINECT% %SRC_KINECT% -o ../../bin/kinect.exe %LIB_KINECT%
	IF %BUILD_CLEAN%==1 CD ../../bin
	IF %BUILD_CLEAN%==1 DEL /Q parrot.exe
	IF %BUILD_CLEAN%==1 DEL /Q kinect.exe
	IF %BUILD_CLEAN%==1 CD ../g++/bin

REM GO BACK TO ORIGINAL DIRECTORY
	CD ../../