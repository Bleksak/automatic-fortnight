#include <stdlib.h>

#include "GL.h"
#include "Image.h"
#include "Shader.h"
#include "Input.h"

#include <cglm/cglm.h>

void game_change_state(struct GL* gl, enum GAME_STATE state) {
    gl->state = state;

    switch(state) {
        case GAME: {
            glfwSetKeyCallback(gl->window, game_keyboard_callback);
            glfwSetCursorPosCallback(gl->window, game_mouse_callback);
            glfwSetMouseButtonCallback(gl->window, game_mouse_button_callback);

            glfwSetInputMode(gl->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            
            if (glfwRawMouseMotionSupported())
                glfwSetInputMode(gl->window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
        } break;

        case INVENTORY: {
            // glfwSetKeyCallback(gl->window, GameKeyCallbackFun);
            // glfwSetCursorPosCallback(gl->window, 0);
            // glfwSetMouseButtonCallback(gl->window, GameMouseButtonCallbackFun);

            glfwSetInputMode(gl->window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        } break;

        case MENU: {
            glfwSetKeyCallback(gl->window, 0);
            glfwSetCursorPosCallback(gl->window, 0);
            glfwSetMouseButtonCallback(gl->window, 0);

            glfwSetInputMode(gl->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        } break;

        case LOAD: {
            glfwSetKeyCallback(gl->window, 0);
            glfwSetCursorPosCallback(gl->window, 0);
            glfwSetMouseButtonCallback(gl->window, 0);

            glfwSetInputMode(gl->window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        } break;

        case GAME_MENU: {
            glfwSetKeyCallback(gl->window, 0);
            glfwSetCursorPosCallback(gl->window, 0);
            glfwSetMouseButtonCallback(gl->window, 0);

            glfwSetInputMode(gl->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        } break;
    }
}

struct GLOption gl_init(const char* game_name) {
    if(!glfwInit()) {
        return Err("OpenGL initialization failed");
    }

    struct GL* gl = (struct GL*)calloc(1, sizeof(struct GL));
    
    gl->monitor = glfwGetPrimaryMonitor();
    gl->mode = glfwGetVideoMode(gl->monitor);
    
    struct GLOption wndOption = window_create(game_name, gl->monitor, gl->mode, false);

    if(!wndOption.ok) {
        free(gl);
        return wndOption;
    }

    gl->window = unwrap(GLFWwindow *, wndOption);
    glfwSetWindowUserPointer(gl->window, gl);

    gl->last_mouse_position[0] = 0.0f;
    gl->last_mouse_position[1] = 0.0f;

    struct GLOption ShaderOption = shader_create("shaders/vertex.glsl", "shaders/fragment.glsl");

    if(!ShaderOption.ok) {
        free(gl);
        return ShaderOption;
    }

    gl->shader = unwrap(struct Shader*, ShaderOption);

    GLFWimage img;
    int trash;
    img.pixels = image_read("icons/curs_p.png", &img.width, &img.height, &trash, 0, false);
    
    if(!img.pixels) {
        free(gl);
        
        Err("Failed to load assets");
    }

    gl->cursor = glfwCreateCursor(&img, 0, 0);
    glfwSetCursorPos(gl->window, 0.0, 0.0);
    image_free(img.pixels);

    glfwSetCursor(gl->window, gl->cursor);

    glUseProgram(gl->shader->id);
    
    gl->first_mouse = true;
    gl->camera = camera_create((float[]){0.0f, 67.0f, 0.0f});

    mat4 projection;
    glm_perspective(glm_rad(90.0f), (float) gl->mode->width / (float) gl->mode->height, 0.1f, 100.0f, projection);
    
    glUniformMatrix4fv(gl->shader->projection_position, 1, GL_FALSE, &projection[0][0]);

    game_change_state(gl, GAME);

    return Ok(gl);
}
