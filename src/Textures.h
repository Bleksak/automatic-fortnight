#ifndef _TEXTURES_H
#define _TEXTURES_H

#include "GL.h"
#include <cglm/cglm.h>

struct TexImageStrData {
    char* xpos;
    char* xneg;
    char* ypos;
    char* yneg;
    char* zpos;
    char* zneg;
};

struct GLOption textures_load(const char* filename);
struct GLOption textures_block_load(char* data[6]);

__attribute__((always_inline)) void texture_bind(GLuint* textures, unsigned int id);

#endif
