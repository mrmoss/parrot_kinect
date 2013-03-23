

SRC=raw_to_jpeg.cpp \
	libfree/cyber_kinect.cpp \
    msl/sprite.cpp \
    msl/socket.cpp \
    msl/file_util.cpp \
    msl/socket_util.cpp \
    msl/string_util.cpp

LIBS=	-pthread \
    -ljpeg \
    -lz \
    -lavcodec \
    -lswscale \
    -lavutil \
    -lfreenect \
    -lGLEW \
    -lglut \
	-lSOIL \
	-lSDL

OPTS=-O2
CFLAGS=-Wall $(OPTS)
COMPILE=g++

all: drone kinect



drone: $(SRC) drone.cpp helisimple/*.cpp
	$(COMPILE) $(CFLAGS) $(SRC) drone.cpp helisimple/*.cpp $(LIBS) -o $@

kinect:  $(SRC) kinect.cpp
	$(COMPILE) $(CFLAGS) $(SRC) kinect.cpp $(LIBS) -o $@

test: kinect
	./kinect

clean:
	- rm drone kinect


# Usage: sudo make apt
apt:
	apt-get install libsoil-dev freeglut3-dev libglu1-mesa-dev libavcodec-dev libswscale-dev libsdl1.2-dev libjpeg62-dev liblz-dev libxi-dev libglew1.5-dev
	add-apt-repository ppa:floe/libtisch
	apt-get update
	apt-get install libfreenect libfreenect-dev libfreenect-demos

# You also need to add yourself to the "video" group.
#     sudo adduser olawlor video

