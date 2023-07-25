#include "camera.h"
#include <cglm/vec3.h>

status_t camera_create(camera_t * camera, float FOV) {
    if (camera == NULL) {
        fprintf(stderr, "camera_t * 'camera' cannot be NULL when being passed to camera_create(...)! ");
        return CAMERA_FAILURE;
    }

    if (FOV < 60 || FOV > 100) {
        fprintf(stderr, "float 'FOV' must be in the range (60 - 100)! ");
        return CAMERA_FAILURE;
    }

    camera->yaw = -90;
    camera->pitch = 0;
    
    camera->movementSpeed = 1.0;
    camera->panSpeed = 1.0;

    camera->FOV = FOV;
    camera->nearPlane = 0.01;
    camera->farPlane = 1000.0f;

    glm_vec3_copy((vec3){0, 0, 0}, camera->position);
    glm_vec3_copy((vec3){0, 0, -1}, camera->front);
    glm_vec3_copy((vec3){1, 0, 0}, camera->right);
    glm_vec3_copy((vec3){0, 1, 0}, camera->up);

    glm_mat4_copy(GLM_MAT4_IDENTITY, camera->viewMatrix);
    glm_mat4_copy(GLM_MAT4_IDENTITY, camera->projMatrix);

    return CAMERA_SUCCESS;
}

status_t camera_move_front(camera_t * camera, float deltaTime) {
    if (camera == NULL) {
        fprintf(stderr, "camera_t * 'camera' cannot be NULL when being passed to camera_move(...)! ");
        return CAMERA_FAILURE;
    }

    vec3 move;

    glm_vec3_copy(camera->front, move);

    glm_vec3_scale(move, deltaTime, move);

    glm_vec3_add(camera->position, move, camera->position);

    return CAMERA_SUCCESS;
}

status_t camera_move_left(camera_t * camera, float deltaTime) {
    if (camera == NULL) {
        fprintf(stderr, "camera_t * 'camera' cannot be NULL when being passed to camera_move(...)! ");
        return CAMERA_FAILURE;
    }

    vec3 move;

    glm_vec3_copy(camera->right, move);

    glm_vec3_scale(move, deltaTime, move);

    glm_vec3_negate(move);

    glm_vec3_add(camera->position, move, camera->position);

    return CAMERA_SUCCESS;
}

status_t camera_move_back(camera_t * camera, float deltaTime) {
    if (camera == NULL) {
        fprintf(stderr, "camera_t * 'camera' cannot be NULL when being passed to camera_move(...)! ");
        return CAMERA_FAILURE;
    }

    vec3 move;

    glm_vec3_copy(camera->front, move);
    
    glm_vec3_scale(move, deltaTime, move);

    glm_vec3_negate(move);

    glm_vec3_add(camera->position, move, camera->position);

    return CAMERA_SUCCESS;
}

status_t camera_move_right(camera_t * camera, float deltaTime) {
    if (camera == NULL) {
        fprintf(stderr, "camera_t * 'camera' cannot be NULL when being passed to camera_move(...)! ");
        return CAMERA_FAILURE;
    }

    vec3 move;

    glm_vec3_copy(camera->right, move);

    glm_vec3_scale(move, deltaTime, move);

    glm_vec3_add(camera->position, move, camera->position);

    return CAMERA_SUCCESS;
}

status_t camera_pan(camera_t * camera, vec3 deltaLook, float deltaTime) {
    if (camera == NULL) {
        fprintf(stderr, "camera_t * 'camera' cannot be NULL when being passed to camera_pan(...)! ");
        return CAMERA_FAILURE;
    }

    if (deltaLook == NULL) {
        fprintf(stderr, "vec3 'deltaLook' cannot be NULL when being passed to camera_pan(...)! ");
        return CAMERA_FAILURE;
    }

    glm_vec3_scale(deltaLook, deltaTime, deltaLook);

    glm_vec3_add(camera->front, deltaLook, camera->front);

    return CAMERA_SUCCESS;
}

status_t camera_update(camera_t * camera, int width, int height) {
    if (camera == NULL) {
        fprintf(stderr, "camera_t * 'camera' cannot be NULL when being passed to camera_update(...)! ");
        return CAMERA_FAILURE;
    }

    glm_normalize(camera->front);

    glm_normalize(camera->up);

    glm_vec3_crossn(camera->front, camera->up, camera->right);

    vec3 pos_front;
    glm_vec3_add(camera->position, camera->front, pos_front);

    glm_lookat(camera->position, pos_front, camera->up, camera->viewMatrix);

    glm_perspective(glm_rad(camera->FOV), (float)width / (float)height, camera->nearPlane, camera->farPlane, camera->projMatrix);

    return CAMERA_SUCCESS;
}