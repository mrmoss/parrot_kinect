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