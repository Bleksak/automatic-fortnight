#pragma once
#include <stdbool.h>

unsigned char* image_read(const char* file, int* width, int* height, int* num_channels, int channels, bool flip);
void image_free(unsigned char* img);


