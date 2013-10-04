OPTS=-O
COMPILER=g++
OUT=bin
SRC_MSL=src/msl/2d.cpp src/msl/2d_util.cpp src/msl/file_util.cpp src/msl/font.cpp src/msl/glut_input.cpp src/msl/socket.cpp src/msl/socket_util.cpp src/msl/sprite.cpp src/msl/string_util.cpp src/msl/time_util.cpp src/msl/webserver.cpp src/msl/webserver_threaded.cpp
SRC_PARROT=$(SRC_MSL) src/main.cpp src/falconer.cpp src/raw_to_jpeg.cpp src/kinect/libfree/*.cpp src/kinect/Kinect.cpp src/PDController.cpp src/PIDController.cpp
CFLAGS_PARROT=-Wall -std=c++0x $(OPTS) -pthread -lm
LIB_PARROT=-lGL -lGLU -lGLEW -lglut -lSOIL -lavcodec -lswscale -ljpeg -lfreenect -lavformat -lavutil -lpthread

SRC_KINECT=src/kinect/main.cpp src/kinect/Kinect.cpp src/kinect/libfree/*.cpp
CFLAGS_KINECT=-Wall $(OPTS)
LIB_KINECT=-lGL -lGLU -lGLEW -lglut -lfreenect -lpthread

all: parrot kinect

parrot: $(SRC_PARROT)
	$(COMPILER) $(CFLAGS_PARROT) $(SRC_PARROT) -o $(OUT)/$@ $(LIB_PARROT)

kinect: $(SRC_KINECT)
	$(COMPILER) $(CFLAGS_KINECT) $(SRC_KINECT) -o $(OUT)/$@ $(LIB_KINECT)

clean:
	rm -f $(OUT)/$@
