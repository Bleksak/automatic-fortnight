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
