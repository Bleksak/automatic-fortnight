#pragma once
#include "Textures.h"

struct Blocks {
    unsigned long long* ids;
    char** names;
    GLuint vao;
    GLuint vbo;
    GLuint vbo_textures;
    GLuint ebo;

    GLuint* textures;
    unsigned int draw_count;
    unsigned long long length;
};

struct GLOption blocks_load(const char* file);
void block_draw(struct Blocks* blocks, unsigned int id);

