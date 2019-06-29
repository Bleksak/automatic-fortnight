#ifndef _BLOCK_H
#define _BLOCK_H

#include "GL.h"

#include "include/cglm/cglm.h"

#include "Objects.h"

enum BLOCK_IDS
{
    AIR,
    STONE,
    COBBLESTONE,
    GRASS,

    BLOCK_ID_SIZE,
};

struct Block
{
    GLuint vao;
    GLuint vbo_vertex, vbo_normal, vbo_texture;
    GLuint ebo;
    vec3 position;
    mat4 model;


    unsigned int object_idx;
    struct Objects* objects;
};

struct Block* CreateBlock(vec3 position, struct Objects* objects, unsigned int block_id);
void DrawBlock(struct Block* block, struct Shader* shader);

#endif