//Code based on jpeg_sample.c from Junaed Sattar at http://www.cim.mcgill.ca/~junaed/libjpeg.php

#include "raw_to_jpeg.h"
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>

#include <math.h> /* for fmod, sqrt */
#include <stdio.h> /* for FILE, used in jpeglib.h */
#include <stdlib.h> /* for size_t */
#include <jpeglib.h> /*for JPEG structs*/

#include <string.h>

// Called by compression machinery to set things up
METHODDEF(void)
my_init_destination (j_compress_ptr cinfo)
{
  jpegDestBuffer * dest = (jpegDestBuffer *) cinfo->dest;
  dest->reset();
}
// Called when our buffer is full
METHODDEF(int)
my_empty_output_buffer (j_compress_ptr cinfo)
{
  jpegDestBuffer * dest = (jpegDestBuffer *) cinfo->dest;
  dest->addOutput(dest->buffer,jpegDestBuffer::BUFSIZE);
  dest->reset();
  return 1;
}
// Called with a half-full buffer at the end of the image
METHODDEF(void)
my_term_destination (j_compress_ptr cinfo)
{
  jpegDestBuffer * dest = (jpegDestBuffer *) cinfo->dest;
  size_t datacount = jpegDestBuffer::BUFSIZE - dest->free_in_buffer;
  dest->addOutput(dest->buffer,datacount);
  dest->reset();
}

jpegDestBuffer::jpegDestBuffer()
{
  init_destination = my_init_destination;
  empty_output_buffer = my_empty_output_buffer;
  term_destination = my_term_destination;
  reset();
}

std::vector<unsigned char> raw_to_jpeg_array(unsigned char* raw_image, int width, int height,
                 int bytes_per_pixel, int color_space)
{
// Standard JPEG compression setup
	jpeg_compress_struct j;
	memset(&j,0,sizeof(j));
	jpeg_create_compress(&j);
	struct jpeg_error_mgr jerr;
	memset(&jerr,0,sizeof(jerr));
	j.err=jpeg_std_error(&jerr);

	// Connect our output buffer (this replaces jpeg_stdio_dest)
	jpegDestBuffer dest;
	j.dest=&dest;

	// Standard JPEG data dump
	const int w=width,h=height;
	j.image_width=w; j.image_height=h;
	j.input_components=bytes_per_pixel;
	j.in_color_space=(J_COLOR_SPACE)color_space;
	jpeg_set_defaults(&j);
	jpeg_set_quality(&j,85,TRUE); // 85% quality
	jpeg_start_compress(&j,TRUE);
	for (int y=0;y<h;y++)
	{
		/*JSAMPLE row[3*w]; // synthetic image data
		for (int x=0;x<w;x++) {
			float r=sqrt(x*x+y*y); // circular pattern
			JSAMPLE g=fmod(r,50.0)*5.0; // cyan stripes
			row[3*x+0]=200*fabs(sin(y*0.1)); // red waves
			row[3*x+1]=g;
			row[3*x+2]=g;
		}*/
		JSAMPLE *rowPointer[1];
		rowPointer[0]=&raw_image[j.next_scanline * j.image_width *  j.input_components]; //row[0];
		jpeg_write_scanlines(&j,rowPointer,1);
	}
	jpeg_finish_compress(&j);
	jpeg_destroy_compress(&j);

	// Dump the finished output buffer
	//std::cout<<"Accumulated "<<dest.output.size()<<" bytes of binary JPEG data.\n";
	//std::ofstream out("out.jpg",std::ios_base::binary);
	//out.write((char *)&dest.output[0],dest.output.size());

	return dest.output;
}

