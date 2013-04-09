//Code based on jpeg_sample.c from Junaed Sattar at http://www.cim.mcgill.ca/~junaed/libjpeg.php

#include "raw_to_jpeg.h"
#include <sstream>
#include <vector>
#include <iostream>

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

#define MYBUFSIZ 20971520

std::string raw_to_jpeg_string(const std::string& filename, unsigned char* raw_image, int width, int height,
                 int bytes_per_pixel, int color_space)
{
	std::cout << "Good so far" << std::endl;
    struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;

	/* this is a pointer to one row of image data */
	JSAMPROW row_pointer[1];


	char b[MYBUFSIZ];
	FILE *outfile = fopen( filename.c_str(), "wb" );
	setbuf(outfile, b);

	std::cout << "Good so far" << std::endl;
	if ( !outfile )
	{
		printf("Error opening output jpeg file %s\n!", filename.c_str() );
		std::cout << "Error opening output jpeg" << std::endl;
		return "";
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
	return std::string(b);
}

