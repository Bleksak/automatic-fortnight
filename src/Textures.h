#ifndef _TEXTURES_H
#define _TEXTURES_H

#include "GL.h"
#include "include/cglm/cglm.h"

struct TexImageStrData
{
    char* PositiveX;
    char* NegativeX;
    char* PositiveY;
    char* NegativeY;
    char* PositiveZ;
    char* NegativeZ;
};

struct GLOption LoadTextures(const char* filename);
struct GLOption LoadBlockTextures(char* data[6]);

__attribute__((always_inline)) void BindTexture(GLuint* textures, unsigned int id);

#endif