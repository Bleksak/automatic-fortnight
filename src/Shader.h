#ifndef _SHADER_H
#define _SHADER_H

#include "GL.h"

struct Shader
{
    GLuint id;
    GLint view_position, model_position, projection_position;
};

struct GLOption CreateShader(const char* vertex, const char* fragment);
void ShaderSetUniformInt(GLuint shader, const char* name, int value);
void ShaderSetUniformMat4(GLuint shader, const char* name, const float* value);

// void ShaderSetUniformMat4FromLocation(GLuint location, const float* value);

#endif