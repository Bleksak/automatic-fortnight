#pragma once

#include "GL.h"
#include <cglm/cglm.h>

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
