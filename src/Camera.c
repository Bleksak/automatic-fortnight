#include "Camera.h"

void camera_update(struct Camera* camera) {
    versor aroundX, aroundY;

    glm_quat(aroundX, glm_rad(camera->pitch), 1.0f, 0.0f, 0.0f);
    glm_quat(aroundY, glm_rad(-camera->yaw), 0.0f, 1.0f, 0.0f);
    
    glm_quat_mul(aroundY, aroundX, camera->orientation);
}

void camera_update_view_matrix(struct Camera* camera) {
    versor reverse_orientation;

    glm_quat_conjugate(camera->orientation, reverse_orientation);
    glm_quat_mat4(reverse_orientation, camera->view);

    camera->view[3][0] = -(camera->view[0][0] * camera->position[0] + camera->view[1][0] * camera->position[1] + camera->view[2][0] * camera->position[2]);
    camera->view[3][1] = -(camera->view[0][1] * camera->position[0] + camera->view[1][1] * camera->position[1] + camera->view[2][1] * camera->position[2]);
    camera->view[3][2] = -(camera->view[0][2] * camera->position[0] + camera->view[1][2] * camera->position[1] + camera->view[2][2] * camera->position[2]); 
    camera->view[3][3] = 1;
}

void camera_mouse_callback(GLFWwindow* window, double xpos, double ypos) {
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

void camera_mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    (void) window;
    (void) button;
    (void) action;
    (void) mods;
}

void camera_keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    (void) scancode;
    (void) action;
    (void) mods;

    struct GL* gl = glfwGetWindowUserPointer(window);

    float velocity = 100.0f * gl->delta_time;

    versor qF;
    versor reverse_orientation;

    glm_quat_conjugate(gl->camera->orientation, reverse_orientation);
    glm_quat_mul_sse2(gl->camera->orientation, (versor){0.0f, 0.0f, -1.0f, 0.0f}, qF);
    glm_quat_mul_sse2(qF, reverse_orientation, qF);

    vec3 front = {qF[0], qF[1], qF[2]};
    vec3 right;

    glm_vec3_crossn(front, (vec3){0.0f, 1.0f, 0.0f}, right);

    vec3 prod;

    switch(key) {
        case GLFW_KEY_W: {
            glm_vec3_scale(front, velocity, prod);
        } break;
    
        case GLFW_KEY_S: {
            glm_vec3_scale(front, -velocity, prod);
        } break;

        case GLFW_KEY_A: {
            glm_vec3_scale(right, -velocity, prod);
        } break;

        case GLFW_KEY_D: {
            glm_vec3_scale(right, velocity, prod);
        } break;

        default: {
            return;
        }
    }

    glm_vec3_add(gl->camera->position, prod, gl->camera->position);

    camera_update(gl->camera);
}

struct Camera* camera_create(vec3 position) {
    struct Camera* camera = (struct Camera*) calloc(1, sizeof(struct Camera));
    glm_vec3_copy(position, camera->position);

    camera->orientation[0] = 0.0f;
    camera->orientation[1] = 0.0f;
    camera->orientation[2] = 0.0f;
    camera->orientation[3] = -1.0f;

    camera->yaw = 45.0f;
    camera->pitch = 45.0f;

    return camera;
}
