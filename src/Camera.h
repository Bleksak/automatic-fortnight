#ifndef _CAMERA_H
#define _CAMERA_H

#include "GL.h"
#include "include/cglm/cglm.h"

void CameraMouseCallback(GLFWwindow* window, double xpos, double ypos);
void CameraMouseBtnCallback(GLFWwindow* window, int button, int action, int mods);
void CameraKeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

struct Camera
{
    vec3 Position;
    versor Orientation;
    float yaw;
    float pitch;

    mat4 view;
};

struct Camera* CreateCamera(vec3 position);
void UpdateCamera(struct Camera* camera);
void UpdateViewMatrix(struct Camera* camera);

#endif