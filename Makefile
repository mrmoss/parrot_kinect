OPTS=-O
COMPILER=g++
OUT=bin
SRC_PARROT=src/main.cpp src/msl/*.cpp src/falconer.cpp src/raw_to_jpeg.cpp src/kinect/libfree/*.cpp src/kinect/Kinect.cpp src/PDController.cpp src/PIDController.cpp
CFLAGS_PARROT=-Wall -std=c++0x $(OPTS)
LIB_PARROT=-lGL -lGLU -lGLEW -lglui -lglut -lSOIL -lavcodec -lswscale -ljpeg -lfreenect

SRC_KINECT=src/kinect/main.cpp src/kinect/Kinect.cpp src/kinect/libfree/*.cpp
CFLAGS_KINECT=-Wall $(OPTS)
LIB_KINECT=-lGL -lGLU -lGLEW -lglui -lglut -lSOIL -lfreenect

all: parrot kinect

parrot: $(SRC_PARROT)
	$(COMPILER) $(CFLAGS_PARROT) $(SRC_PARROT) -o $(OUT)/$@ $(LIB_PARROT)

kinect: $(SRC_KINECT)
	$(COMPILER) $(CFLAGS_KINECT) $(SRC_KINECT) -o $(OUT)/$@ $(LIB_KINECT)

clean:
	rm -f $(OUT)/$@
