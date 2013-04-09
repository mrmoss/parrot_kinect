#include <stdio.h>
#include <jpeglib.h>
#include <stdlib.h>

#include <string>

#ifndef RAW_TO_JPEG_H_
#define RAW_TO_JPEG_H_

bool raw_to_jpeg(const std::string& filename, unsigned char* raw_image, int width, int height,
                 int bytes_per_pixel, int color_space);

std::string raw_to_jpeg_string(const std::string& filename, unsigned char* raw_image, int width, int height,
                        int bytes_per_pixel, int color_space);
#endif //RAW_TO_JPEG_H_
