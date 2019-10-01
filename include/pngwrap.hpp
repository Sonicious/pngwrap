#pragma once

/*
simple Wrapper for PNG Library.

Features:
 - Reading 8 bit quadratic gray-valued images
 - Writing 8 bit gray valued images

Todo:
 - More use of pngstate-enum
 - read RGB images
 - Grayscale conversion of gray-valued images
 - ....much more

*/

enum pngstate{
    PNG_WRAP_SUCCESS = 0b00,
    PNG_WRAP_FAILURE = 0b01,
    PNG_WRAP_WARNING = 0b10
};

void readpng_version_info(void);

int imread(const char* filename, unsigned char** image, unsigned int *width, unsigned int *height);
int imwrite(const char* filename, unsigned char* image, unsigned int width, unsigned int height);