#ifndef _OBJECT_H
#define _OBJECT_H

#include "Shader.h"
#include "Textures.h"
#include "GL.h"

struct Material
{
    unsigned int id;
    unsigned int block_count;
};

struct Materials
{
    struct Material* materials;
    unsigned int length;
};

struct Object
{
    vec3* v;
    vec2* vt;
    vec3* vn;
    unsigned int *f;

    unsigned long long f_size;
    struct Materials materials;
};

struct Objects
{
    struct Object* objects;
    unsigned long long length;
    const struct Textures* textures;
};

struct GLOption LoadObjects(const char* filename);
void RenderObject(struct Object* object, GLuint shader);

#endif