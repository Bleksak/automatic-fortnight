#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "include/stb/stb_image.h"

unsigned char* ReadImage(const char* file, int* width, int* height, int* num_channels, int channels, bool flip)
{
    stbi_set_flip_vertically_on_load(flip);
    return stbi_load(file, width, height, num_channels, channels);
}

void FreeImage(unsigned char* img)
{
    stbi_image_free(img);
}