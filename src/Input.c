#include "Input.h"
#include "Camera.h"
#include <cglm/vec3.h>

void keyCallbackFun(GLFWwindow* window, int key, int scancode, int action, int mods) {
    (void) window;
    (void) key;
    (void) scancode;
    (void) action; 
    (void) mods;
}

void mouseMoveCallbackFun(GLFWwindow* window, double xpos, double ypos) {
    (void) window;
    (void) xpos;
    (void) ypos;
}

void mouseButtonCallbackFun(GLFWwindow* window, int button, int action, int mods) {
    (void) window;
    (void) button;
    (void) action;
    (void) mods;
    (void) window;
}

void game_mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    struct GL* gl = glfwGetWindowUserPointer(window);

    if(gl->first_mouse) {
        gl->last_mouse_position[0] = (float) xpos;
        gl->last_mouse_position[1] = (float) ypos;
        gl->first_mouse = false;
    }

    float xoffset = ((float)xpos - gl->last_mouse_position[0]);
    float yoffset = (gl->last_mouse_position[1] - (float)ypos);

    gl->last_mouse_position[0] = xpos;
    gl->last_mouse_position[1] = ypos;

    float sensitivity = 0.3f;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    gl->camera->yaw += xoffset;
    gl->camera->pitch += yoffset;

    if(gl->camera->pitch > 90.0f) {
        gl->camera->pitch = 90.0f;
    }

    if(gl->camera->pitch < -90.0f) {
        gl->camera->pitch = -90.0f;
    }

    camera_update(gl->camera);
}

void game_mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    (void) window;
    (void) button;
    (void) action;
    (void) mods;
}

void game_keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    (void) scancode;
    (void) action;
    (void) mods;

    struct GL* gl = glfwGetWindowUserPointer(window);

    // printf("ACTION: %d\n", action);

    gl->input_update_info.ascii_characters[key] = (action != GLFW_RELEASE);

    // float velocity = 100.0f * gl->delta_time;

    // versor qF;
    // versor reverse_orientation;

    // glm_quat_conjugate(gl->camera->orientation, reverse_orientation);
    // glm_quat_mul_sse2(gl->camera->orientation, (versor){0.0f, 0.0f, -1.0f, 0.0f}, qF);
    // glm_quat_mul_sse2(qF, reverse_orientation, qF);

    // vec3 front = {qF[0], 0.0f, qF[2]};

    // glm_vec3_normalize(front);

    // vec3 right;

    // glm_vec3_crossn(front, (vec3){0.0f, 1.0f, 0.0f}, right);

    // float forward = gl->input_update_info.ascii_characters[GLFW_KEY_W] - gl->input_update_info.ascii_characters[GLFW_KEY_S];
    // float up = gl->input_update_info.ascii_characters[GLFW_KEY_SPACE] - gl->input_update_info.ascii_characters[GLFW_KEY_LEFT_SHIFT];
    // float side = gl->input_update_info.ascii_characters[GLFW_KEY_D] - gl->input_update_info.ascii_characters[GLFW_KEY_A];
    
    // vec3 move_vec = {side, up, forward};
    // glm_vec3_scale(move_vec, velocity, move_vec);

    // vec3 upv = {0.0f, 1.0f, 0.0f};

    // glm_vec3_scale(right, move_vec[0], right);
    // glm_vec3_scale(upv, move_vec[1], upv);
    // glm_vec3_scale(front, move_vec[2], front);

    // glm_vec3_addadd(front, right, gl->camera->position);
    // glm_vec3_add(gl->camera->position, upv, gl->camera->position);


    // camera_update(gl->camera);
}

void input_update(struct GL* gl) {

    float velocity = 100.0f * gl->delta_time;

    versor qF;
    versor reverse_orientation;

    glm_quat_conjugate(gl->camera->orientation, reverse_orientation);
    glm_quat_mul_sse2(gl->camera->orientation, (versor){0.0f, 0.0f, -1.0f, 0.0f}, qF);
    glm_quat_mul_sse2(qF, reverse_orientation, qF);

    vec3 front = {qF[0], 0.0f, qF[2]};

    glm_vec3_normalize(front);

    vec3 right;

    glm_vec3_crossn(front, (vec3){0.0f, 1.0f, 0.0f}, right);

    float forward = gl->input_update_info.ascii_characters[GLFW_KEY_W] - gl->input_update_info.ascii_characters[GLFW_KEY_S];
    float up = gl->input_update_info.ascii_characters[GLFW_KEY_SPACE] - gl->input_update_info.ascii_characters[GLFW_KEY_LEFT_SHIFT];
    float side = gl->input_update_info.ascii_characters[GLFW_KEY_D] - gl->input_update_info.ascii_characters[GLFW_KEY_A];
    
    vec3 move_vec = {side, up, forward};
    glm_vec3_scale(move_vec, velocity, move_vec);

    vec3 upv = {0.0f, 1.0f, 0.0f};

    glm_vec3_scale(right, move_vec[0], right);
    glm_vec3_scale(upv, move_vec[1], upv);
    glm_vec3_scale(front, move_vec[2], front);

    glm_vec3_addadd(front, right, gl->camera->position);
    glm_vec3_add(gl->camera->position, upv, gl->camera->position);


    camera_update(gl->camera);
}
