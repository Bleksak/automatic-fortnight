#include <stdlib.h>
#include <stdio.h>

#include "Shader.h"

struct GLOption shader_create(const char* vertex, const char* fragment) {
    FILE* fp = fopen(vertex, "rb");
    
    if(!fp) {
        return Err("Failed to load assets");
    }

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    GLchar* vertex_str = (GLchar*)calloc(1 + (unsigned long long)size, sizeof(GLchar));
    fread(vertex_str, 1, (unsigned long long) size, fp);
    fclose(fp);

    fp = fopen(fragment, "rb");

    if(!fp) {
        free(vertex_str);
        
        return Err("Failed to load assets");
    }


    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    GLchar* fragment_str = (GLchar*)calloc(1 + (unsigned long long)size, sizeof(GLchar));
    fread(fragment_str, 1, (unsigned long long)size, fp);
    fclose(fp);

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    if(!vertex_shader || !fragment_shader) {
        free(fragment_str);
        free(vertex_str);

        return Err("Failed to load assets");
    }

    glShaderSource(vertex_shader, 1, (const GLchar* const*)&vertex_str, 0);
    glCompileShader(vertex_shader);

    int status;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &status);

    if(!status) {
        static char error_log[1024];
        glGetShaderInfoLog(vertex_shader, 1024, 0, error_log);

        return Err(error_log);
    }

    glShaderSource(fragment_shader, 1, (const GLchar* const*)&fragment_str, 0);
    glCompileShader(fragment_shader);

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &status);

    if(!status) {
        static char error_log[1024];
        glGetShaderInfoLog(fragment_shader, 1024, 0, error_log);

        return Err(error_log);
    }

    GLuint ProgramID = glCreateProgram();
    
    if(!ProgramID) {
        return Err("Failed to load assets");
    }

    glAttachShader(ProgramID, vertex_shader);
    glAttachShader(ProgramID, fragment_shader);

    glLinkProgram(ProgramID);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    struct Shader* shader = malloc(sizeof(struct Shader));
    glUseProgram(ProgramID);

    shader->id = ProgramID;

    shader->model_position = glGetUniformLocation(ProgramID, "model");
    shader->view_position = glGetUniformLocation(ProgramID, "view");
    shader->projection_position = glGetUniformLocation(ProgramID, "projection");

    return Ok(shader);
}

void shader_set_uniform_int(GLuint shader, const char* name, int value) {
    glUniform1i(glGetUniformLocation(shader, name), value);
}

void shader_set_uniform_mat4(GLuint shader, const char* name, const float* value) {
    glUniformMatrix4fv(glGetUniformLocation(shader, name), 1, GL_FALSE, value);
}

void shader_set_uniform_mat4_loc(GLuint location, const float* value) {
    glUniformMatrix4fv(location, 1, GL_FALSE, value);
}
