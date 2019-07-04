#ifndef _BLOCKS_H
#define _BLOCKS_H

#include "Textures.h"

struct Blocks
{
    unsigned long long* ids;
    char** names;
    GLuint vao;
    GLuint vbo;
    GLuint vbo_textures;
    GLuint ebo;

    GLuint* textures;
    unsigned long long length;
};

struct GLOption LoadBlocks(const char* file);

#endif