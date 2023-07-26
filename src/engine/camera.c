#include "camera.h"
#include <cglm/vec3.h>

status_t camera_create(camera_t * camera, float FOV, float sensitivity, uint32_t windowWidth, uint32_t windowHeight) {
    if (camera == NULL) {
        fprintf(stderr, "camera_t * 'camera' cannot be NULL when being passed to camera_create(...)! ");
        return CAMERA_FAILURE;
    }

    if (FOV < 60 || FOV > 100) {
        fprintf(stderr, "float 'FOV' must be in the range (60 - 100)! ");
        return CAMERA_FAILURE;
    }

    camera->windowWidth = windowWidth;
    camera->windowHeight = windowHeight;

    camera->firstMouse = true;

    camera->lastX = (float)camera->windowWidth / 2;
    camera->lastY = (float)camera->windowHeight;

    camera->yaw = -90;
    camera->pitch = 0;
    
    camera->movementSpeed = 1.0;
    camera->lookSpeed = sensitivity;
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

status_t camera_look(camera_t * camera, float yaw, float pitch, float deltaTime) {
    if (camera == NULL) {
        fprintf(stderr, "camera_t * 'camera' cannot be NULL when being passed to camera_look(...)! ");
        return CAMERA_FAILURE;
    }

    camera->yaw = yaw;
    camera->pitch = pitch;

    camera->front[0] = cos(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));
    camera->front[1] = sin(glm_rad(camera->pitch));
    camera->front[2] = sin(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));

    glm_vec3_norm(camera->front);

    return CAMERA_SUCCESS;
}

status_t camera_update(camera_t * camera) {
    if (camera == NULL) {
        fprintf(stderr, "camera_t * 'camera' cannot be NULL when being passed to camera_update(...)! ");
        return CAMERA_FAILURE;
    }

    glm_clamp(camera->pitch, -90, 90);

    glm_normalize(camera->front);

    glm_normalize(camera->up);

    glm_vec3_crossn(camera->front, camera->up, camera->right);

    vec3 pos_front;
    glm_vec3_add(camera->position, camera->front, pos_front);

    glm_lookat(camera->position, pos_front, camera->up, camera->viewMatrix);

    glm_perspective(glm_rad(camera->FOV), (float)camera->windowWidth / (float)camera->windowHeight, camera->nearPlane, camera->farPlane, camera->projMatrix);

    return CAMERA_SUCCESS;
}

status_t camera_print(camera_t * camera) {
    if (camera == NULL) {
        fprintf(stderr, "camera_t * 'camera' cannot be NULL when being passed to camera_print(...)! ");
        return CAMERA_FAILURE;
    }

    fprintf(stdout, "Camera Position: [%.3f %.3f %.3f]\n", camera->position[0], camera->position[1], camera->position[2]);
    fprintf(stdout, "Camera Front: [%.3f %.3f %.3f]\n", camera->front[0], camera->front[1], camera->front[2]);
    fprintf(stdout, "Camera Up: [%.3f %.3f %.3f]\n", camera->up[0], camera->up[1], camera->up[2]);
    fprintf(stdout, "Camera Right: [%.3f %.3f %.3f]\n", camera->up[0], camera->up[1], camera->up[2]);
    fprintf(stdout, "Camera Yaw: [%3.f]\n", camera->yaw);
    fprintf(stdout, "Camera Pitch: [%.3f]\n", camera->pitch);
    fprintf(stdout, "Camera dX: [%.3f]\n", camera->currentX);
    fprintf(stdout, "Camera dY: [%.3f]\n", camera->currentY);

    return CAMERA_SUCCESS;
}