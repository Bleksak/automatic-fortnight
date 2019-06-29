#include <stdbool.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Window.h"
#include "Camera.h"
#include "Shader.h"

#ifndef _GL_H
#define _GL_H

struct Vec3f
{
    union
    {
        float a, x;
    };

    union
    {
        float b, y;
    };

    union
    {
        float c, z;
    };
} __attribute__((packed));

struct Vec2f
{
    union
    {
        float a, x;
    };

    union
    {
        float b, y;
    };
} __attribute__((packed));

struct GLOption
{
    bool ok;
    union
    {
        void* result_ptr;
        unsigned long long result_uint64;
        unsigned int result_uint32;
        unsigned short result_uint16;
        unsigned char result_uint8;
        GLuint result_gluint;
        const char* error_message;
    };
};

enum GAME_STATE
{
    MENU,
    GAME,
    LOAD,
    GAME_MENU,
    INVENTORY,
};

struct GL
{
    bool firstMouse;
    float lastX;
    float lastY;

    GLFWmonitor* monitor;
    GLFWcursor* cursor;
    GLFWwindow* window;
    const GLFWvidmode* mode;
    
    enum GAME_STATE state;

    struct Shader* shader;
    struct Camera* camera;

    float deltaTime;
    float lastTime;
};

void game_change_state(struct GL* gl, enum GAME_STATE state);
struct GLOption GLInit(const char* game_name);

#endif