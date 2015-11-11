Parrot Kinect:

	Flies a Parrot ARdrone 2.0 using a
	Microsoft Kinect 360 for indoor localization.

To build:
	make

To set up coordinate system:
0.) Plug in Kinect.
1.) Install libfreenect, and get examples working.
2.) Run bin/kinect
3.) Adjust coordinate system in src/kinect/coordinate_system.h
4.) Recompile and re-run until white region is clear.

To fly:
0.) Put batteries in ARdrone.
1.) Connect to ardrone2 wifi network.
2.) Run bin/parrot
3.) Press 'r' until out of emergency mode.
4.) Press '0' to take off.  Manually align yaw so main camera is facing away from the Kinect.
5.) Verify Kinect is tracking position correctly.
6.) Press 'o' for autonomous position hold.
7.) Press spacebar to land.





Dependencies:
	libfreenect
	libusb
	libjpeg
	libavcodec
	libswscale
	soil
	opengl
	glu
	glut
	glew

Installation:
	On Linux/Mac:
		Install the dependencies.
			Ubuntu - sudo apt-get install libusb-dev libjpeg-dev libavcodec-dev libswscale-dev libsoil-dev freeglut3-dev libglew1.6-dev
			Fedora - sudo yum install libusb-devel libjpeg-devel ffmpeg-devel freeglut-devel mesa-libGLU-devel glew-devel SOIL-devel
			Others - Google search each library, download, install from source.
			Mac    - Same as Ubuntu except use brew or ports.
		Run make.

	On Windows:
		Run make.bat.

Controls:
	Cameras:
		1 - forward camera
		2 - bottom camera

	Motion:
		w - forward
		s - backward
		a - left
		d - right
		UP_ARROW   - ascend
		DOWN_ARROW - descend

	Rotation:
		RIGHT_ARROW - clockwise
		LEFT_ARROW  - counter clockwise

	Misc:
		0 - take off
		SPACE - land
		r - reset parrot
		o - toggle autonomous mode
