#ifndef _TEXTURES_H
#define _TEXTURES_H

#include "GL.h"
#include "include/cglm/cglm.h"

struct Texture
{
    GLuint id;
    vec3 ambient, diffuse, specular;
    float specular_exponent, transparency, optical_density;
};

struct Textures
{
    struct Texture* textures;
    unsigned long long length;
};

struct GLOption LoadTextures(const char* filename);
__attribute__((always_inline)) void BindTexture(const struct Textures* textures, unsigned int id);

#endif