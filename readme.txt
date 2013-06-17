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
		When I can make the binaries for libfreenect...run make (it's a bat file, you do not need to install anything else).
		Still working on collecting the last of the dependencies for Windows...  Currently unsupported because of this...
		Your welcome to build it yourself though if you are pro enough to get libfreenect to actually build...
		In which case could you send me a copy of your binaries?
