@ECHO OFF

set MSL=src/msl/2d.cpp src/msl/2d_util.cpp src/msl/file_util.cpp src/msl/font.cpp src/msl/glut_input.cpp src/msl/socket.cpp src/msl/socket_util.cpp src/msl/sprite.cpp src/msl/string_util.cpp src/msl/time_util.cpp src/msl/webserver.cpp src/msl/webserver_threaded.cpp
set SOIL=windows\mingw\i686-w64-mingw32\include\SOIL\SOIL.c windows\mingw\i686-w64-mingw32\include\SOIL\stb_image_aug.c
SET SRC_PARROT=%MSL% %SOIL% src/main.cpp src/falconer.cpp src/raw_to_jpeg.cpp src/kinect/libfree/*.cpp src/kinect/Kinect.cpp src/PDController.cpp src/PIDController.cpp
set SRC_KINECT=src/kinect/main.cpp src/kinect/Kinect.cpp src/kinect/libfree/*.cpp
set OPTS=-O -Wall -Wno-unknown-pragmas -static-libgcc -static-libstdc++ -std=c++11
set LIB_PARROT=-lopengl32 -lglu32 -lfreeglut -lfreeglut_static -lglew32 -lwsock32 -lavcodec -lavdevice -lavfilter -lavformat -lavutil -lx264 -ltheoraenc -logg -ltheoradec -lswscale -lpthread -lfreenect -lusb -ljpeg -lm
set LIB_KINECT=-lopengl32 -lglu32 -lfreeglut -lfreeglut_static -lglew32 -lpthread -lfreenect -lusb
set COMPILER=windows\mingw\bin\g++.exe
set OUT=bin

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

	IF %BUILD_CLEAN%==1 SET BUILD_PARROT=0
	IF %BUILD_CLEAN%==1 SET BUILD_KINECT=0
	IF %ARGC%==0 SET BUILD_PARROT=1
	IF %ARGC%==0 SET BUILD_KINECT=1

REM BUILD WHAT IS NEEDED
	IF %BUILD_PARROT%==1 %COMPILER% %SRC_PARROT% %LIB_PARROT% %OPTS% -o %OUT%/parrot.exe
	IF %BUILD_KINECT%==1 %COMPILER% %SRC_KINECT% %LIB_KINECT% %OPTS% -o %OUT%/kinect.exe
	IF %BUILD_CLEAN%==1 DEL /Q %OUT%\parrot.exe
	IF %BUILD_CLEAN%==1 DEL /Q %OUT%\kinect.exe