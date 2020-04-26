#ifndef _SHADER_H
#define _SHADER_H

#include "GL.h"

struct Shader {
    GLuint id;
    GLint view_position, model_position, projection_position;
};

struct GLOption shader_create(const char* vertex, const char* fragment);
void shader_set_uniform_int(GLuint shader, const char* name, int value);
void shader_set_uniform_mat4(GLuint shader, const char* name, const float* value);

// void ShaderSetUniformMat4FromLocation(GLuint location, const float* value);

#endif
