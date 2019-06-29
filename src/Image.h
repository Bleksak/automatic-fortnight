#ifndef _IMAGE_H
#define _IMAGE_H

#include <stdbool.h>

unsigned char* ReadImage(const char* file, int* width, int* height, int* num_channels, int channels, bool flip);
void FreeImage(unsigned char* img);


#endif