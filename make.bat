REM COMPILER VARIABLES
	SET OPTS=-O
	SET COMPILER=g++
	SET OUT=../../bin
	SET SRC_PARROT=../../src/main.cpp ../../src/msl/*.cpp ../../src/falconer.cpp ../../src/raw_to_jpeg.cpp ../../src/kinect/libfree/*.cpp ../../src/kinect/Kinect.cpp ../../src/PDController.cpp ../../src/PIDController.cpp
	SET CFLAGS_PARROT=-Wall -std=c++0x %OPTS%
	SET LIB_PARROT=-lGL -lGLU -lGLEW -lglui -lglut -lSOIL -lavcodec -lswscale -ljpeg -lfreenect
	SET SRC_KINECT=../../src/kinect/main.cpp ../../src/kinect/Kinect.cpp ../../src/kinect/libfree/*.cpp
	SET CFLAGS_KINECT=-Wall %OPTS%
	SET LIB_KINECT=-lGL -lGLU -lGLEW -lglui -lglut -lSOIL -lfreenect

REM GO TO COMPILER DIRECTORY
	cd g++/bin

REM GET NUMBER OF COMMAND LINE ARGUMENTS
	set argC=0
	for %%x in (%*) do Set /A argC+=1

REM BUILD VARIABLES
	SET /a BUILD_PARROT=0
	SET /a BUILD_PARROT=0
	SET /a BUILD_CLEAN=0

REM EMPTY MAKE ARGUMENTS MEANS MAKE ALL
	IF %argC%==0 SET /a BUILD_PARROT=1
	IF %argC%==0 SET /a BUILD_KINECT=1

REM CHECK COMMAND LINE OPTIONS
	SET /a i=1
	:LOOP
		IF %i%==%argC% GOTO END
		IF %%i%=="all" SET /a BUILD_PARROT=1
		IF %%i%=="all" SET /a BUILD_KINECT=1
		IF %%i%=="parrot" SET /a BUILD_PARROT=1
		IF %%i%=="kinect" SET /a BUILD_KINECT=1
		IF %%i%=="clean" SET /a BUILD_CLEAN=1
		IF %%i%=="clean" GOTO END
		SET /a i=%i%+1
		GOTO LOOP
	:END

REM BUILD WHAT IS NEEDED
	IF %BUILD_PARROT%==1 %COMPILER% %CFLAGS_PARROT% %SRC_PARROT% -o %OUT%/parrot.exe %LIB_PARROT%
	IF %BUILD_KINECT%==1 %COMPILER% %CFLAGS_KINECT% %SRC_KINECT% -o %OUT%/kinect.exe %LIB_KINECT%
	IF %BUILD_CLEAN%==1	del %OUT%/parrot.exe %OUT%/kinect.exe

REM GO BACK TO ORIGINAL DIRECTORY
	cd ../../
