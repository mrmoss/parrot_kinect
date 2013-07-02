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

bool raw_to_jpeg(const std::string& filename, unsigned char* raw_image, int width, int height,
                 int bytes_per_pixel, int color_space)
{
    struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;

	/* this is a pointer to one row of image data */
	JSAMPROW row_pointer[1];
	FILE *outfile = fopen( filename.c_str(), "wb" );

	if ( !outfile )
	{
		printf("Error opening output jpeg file %s\n!", filename.c_str() );
		std::cout << "Error opening output jpeg" << std::endl;
		return -1;
	}
	cinfo.err = jpeg_std_error( &jerr );
	jpeg_create_compress(&cinfo);
	jpeg_stdio_dest(&cinfo, outfile);

	/* Setting the parameters of the output file here */
	cinfo.image_width = width;
	cinfo.image_height = height;
	cinfo.input_components = bytes_per_pixel;
	cinfo.in_color_space = (J_COLOR_SPACE)color_space;
    /* default compression parameters, we shouldn't be worried about these */
	jpeg_set_defaults( &cinfo );
	/* Now do the compression .. */
	jpeg_start_compress( &cinfo, TRUE );
	/* like reading a file, this time write one row at a time */
	while( cinfo.next_scanline < cinfo.image_height )
	{
		row_pointer[0] = &raw_image[ cinfo.next_scanline * cinfo.image_width *  cinfo.input_components];
		jpeg_write_scanlines( &cinfo, row_pointer, 1 );
	}
	/* similar to read file, clean up after we're done compressing */
	jpeg_finish_compress( &cinfo );
	jpeg_destroy_compress( &cinfo );
	fclose( outfile );
	/* success code is 1! */
	return 1;
}

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

jpegDestBuffer raw_to_jpeg_array(unsigned char* raw_image, int width, int height,
                 int bytes_per_pixel, int color_space)
{
// Standard JPEG compression setup
	jpeg_compress_struct j;
	jpeg_create_compress(&j);
	struct jpeg_error_mgr jerr;
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

	return dest;
}

