#ifndef _CAMERA_H
#define _CAMERA_H

#include "GL.h"
#include <cglm/cglm.h>

void camera_mouse_callback(GLFWwindow* window, double xpos, double ypos);
void camera_mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void camera_keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

struct Camera {
    vec3 position;
    versor orientation;
    float yaw;
    float pitch;

    mat4 view;
};

struct Camera* camera_create(vec3 position);
void camera_update(struct Camera* camera);
void camera_update_view_matrix(struct Camera* camera);

#endif
