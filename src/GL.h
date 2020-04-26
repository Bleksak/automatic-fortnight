#include <stdbool.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cglm/cglm.h>

#include "Window.h"
#include "Camera.h"
#include "Shader.h"

#ifndef _GL_H
#define _GL_H

#define unwrap(T, var) (T) (var.result)

#define Ok(var) (struct GLOption) {.ok = true, .result = (void*) (uintptr_t)var,}
#define Err(msg) (struct GLOption) {.ok = false, .error_message = msg,};

struct GLOption {
    bool ok;
    union {
        const char* error_message;
        void* result;
    };
};

enum GAME_STATE {
    MENU,
    GAME,
    LOAD,
    GAME_MENU,
    INVENTORY,
};

struct GL {
    bool first_mouse;
    vec2 last_mouse_position;

    GLFWmonitor* monitor;
    GLFWcursor* cursor;
    GLFWwindow* window;
    const GLFWvidmode* mode;
    
    enum GAME_STATE state;

    struct Shader* shader;
    struct Camera* camera;

    float delta_time;
    float last_time;
};

void game_change_state(struct GL* gl, enum GAME_STATE state);
struct GLOption gl_init(const char* game_name);

#endif
