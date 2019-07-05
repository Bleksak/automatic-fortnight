#include "Camera.h"

void UpdateCamera(struct Camera* camera)
{
    versor aroundX, aroundY;

    glm_quat(aroundX, glm_rad(camera->pitch), 1.0f, 0.0f, 0.0f);
    glm_quat(aroundY, glm_rad(-camera->yaw), 0.0f, 1.0f, 0.0f);
    
    glm_quat_mul(aroundY, aroundX, camera->Orientation);
}

void UpdateViewMatrix(struct Camera* camera)
{
    versor reverseOrientation;

    glm_quat_conjugate(camera->Orientation, reverseOrientation);
    glm_quat_mat4(reverseOrientation, camera->view);

    camera->view[3][0] = -(camera->view[0][0] * camera->Position[0] + camera->view[1][0] * camera->Position[1] + camera->view[2][0] * camera->Position[2]);
    camera->view[3][1] = -(camera->view[0][1] * camera->Position[0] + camera->view[1][1] * camera->Position[1] + camera->view[2][1] * camera->Position[2]);
    camera->view[3][2] = -(camera->view[0][2] * camera->Position[0] + camera->view[1][2] * camera->Position[1] + camera->view[2][2] * camera->Position[2]); 
    camera->view[3][3] = 1;
}

void CameraMouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    struct GL* gl = glfwGetWindowUserPointer(window);

    if(gl->firstMouse)
    {
        gl->lastX = (float)xpos;
        gl->lastY = (float)ypos;
        gl->firstMouse = false;
    }

    float xoffset = ((float)xpos - gl->lastX);
    float yoffset = (gl->lastY - (float)ypos);

    gl->lastX = xpos;
    gl->lastY = ypos;

    float sensitivity = 0.3f;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    gl->camera->yaw += xoffset;
    gl->camera->pitch += yoffset;

    if(gl->camera->pitch > 90.0f)
    {
        gl->camera->pitch = 90.0f;
    }

    if(gl->camera->pitch < -90.0f)
    {
        gl->camera->pitch = -90.0f;
    }

    UpdateCamera(gl->camera);
}

void CameraMouseBtnCallback(GLFWwindow* window, int button, int action, int mods)
{
    (void) window;
    (void) button;
    (void) action;
    (void) mods;
}

void CameraKeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    (void) scancode;
    (void) action;
    (void) mods;

    struct GL* gl = glfwGetWindowUserPointer(window);

    float velocity = 100.0f * gl->deltaTime;

    versor qF;
    versor reverseOrientation;

    glm_quat_conjugate(gl->camera->Orientation, reverseOrientation);
    glm_quat_mul_sse2(gl->camera->Orientation, (versor){0.0f, 0.0f, -1.0f, 0.0f}, qF);
    glm_quat_mul_sse2(qF, reverseOrientation, qF);

    vec3 Front = {qF[0], qF[1], qF[2]};
    vec3 Right;

    glm_vec3_crossn(Front, (vec3){0.0f, 1.0f, 0.0f}, Right);

    vec3 prod;

    switch(key)
    {
        case GLFW_KEY_W:
        {
            glm_vec3_scale(Front, velocity, prod);
        } break;
    
        case GLFW_KEY_S:
        {
            glm_vec3_scale(Front, -velocity, prod);
        } break;

        case GLFW_KEY_A:
        {
            glm_vec3_scale(Right, -velocity, prod);
        } break;

        case GLFW_KEY_D:
        {
            glm_vec3_scale(Right, velocity, prod);
        } break;

        default:
        {
            return;
        }
    }

    glm_vec3_add(gl->camera->Position, prod, gl->camera->Position);

    UpdateCamera(gl->camera);
}

struct Camera* CreateCamera(vec3 position)
{
    struct Camera* camera = (struct Camera*) calloc(1, sizeof(struct Camera));
    glm_vec3_copy(position, camera->Position);

    camera->Orientation[0] = 0.0f;
    camera->Orientation[1] = 0.0f;
    camera->Orientation[2] = 0.0f;
    camera->Orientation[3] = -1.0f;

    camera->yaw = 45.0f;
    camera->pitch = 45.0f;

    return camera;
}