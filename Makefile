

SRC=libfree/cyber_kinect.cpp

LIBS=	-pthread \
    -ljpeg \
    -lz \
    -lavcodec \
    -lswscale \
    -lavutil \
    -lfreenect \
    -lGLEW \
    -lglut \
	-lSDL \
	-lSOIL \
	-lglui

OPTS=-O2
CFLAGS=-Wall -Wno-strict-aliasing $(OPTS)
COMPILE=g++

all: drone kinect



drone: $(SRC) drone.cpp helisimple/*.cpp msl/*.cpp
	$(COMPILE) $(CFLAGS) $(SRC) drone.cpp helisimple/*.cpp msl/*.cpp $(LIBS) -o $@

kinect:  $(SRC) kinect.cpp
	$(COMPILE) $(CFLAGS) $(SRC) kinect.cpp $(LIBS) -o $@

test: kinect
	./kinect

clean:
	- rm drone kinect


# Usage: sudo make apt
apt:
	apt-get install libsoil-dev freeglut3-dev libglu1-mesa-dev libavcodec-dev libsdl1.6-dev libswscale-dev libjpeg62-dev liblz-dev libxi-dev libglew1.5-dev libglui-dev
	add-apt-repository ppa:floe/libtisch
	apt-get update
	apt-get install libfreenect libfreenect-dev libfreenect-demos

# You also need to add yourself to the "video" group.
#     sudo adduser olawlor video

