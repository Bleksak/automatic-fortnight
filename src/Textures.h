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
void LoadBlockTextures(GLuint* textures, struct TexImageStrData* data);

__attribute__((always_inline)) void BindTexture(GLuint* textures, unsigned int id);

#endif