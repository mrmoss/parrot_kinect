#include <stdio.h>
#include <jpeglib.h>
#include <stdlib.h>

#include <string>
#include <vector>

#ifndef RAW_TO_JPEG_H_
#define RAW_TO_JPEG_H_

/* This is a libjpeg memory output buffer.
   It's used to write a JPEG to an in-memory std::vector.
   jpeg_destination_manager is from libjpeg.h
*/
class jpegDestBuffer :  public jpeg_destination_mgr {
public:
  // Stores finished compressed binary output data
  std::vector<unsigned char> output;
  void addOutput(const JOCTET *src,unsigned long n) {
     while (n-->0) output.push_back(*src++);
  }

  // Temporarily buffers output data during write
  enum {BUFSIZE=4096};
  JOCTET buffer[BUFSIZE];
  void reset(void) {
  	next_output_byte=buffer;
  	free_in_buffer=BUFSIZE;
  }

  jpegDestBuffer();
};

bool raw_to_jpeg(const std::string& filename, unsigned char* raw_image, int width, int height,
                 int bytes_per_pixel, int color_space);

jpegDestBuffer raw_to_jpeg_array(unsigned char* raw_image, int width, int height,
                 int bytes_per_pixel, int color_space);

#endif //RAW_TO_JPEG_H_
