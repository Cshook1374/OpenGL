#ifndef __CAMERA_H__
#define __CAMERA_H__

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

#include <cglm/cglm.h>

#include <stdio.h>
#include <stdlib.h>

#define CAMERA_SUCCESS 0
#define CAMERA_FAILURE -1

typedef int32_t status_t;

typedef struct {
    float FOV;
    float movementSpeed;
    float panSpeed;
    float nearPlane;
    float farPlane;
    float yaw;
    float pitch;
    vec3 position;
    vec3 front;
    vec3 right;
    vec3 up;
    mat4 viewMatrix;
    mat4 projMatrix;
} camera_t;

status_t camera_create(camera_t * camera, float FOV);

status_t camera_move_front(camera_t * camera, float deltaTime);
status_t camera_move_left(camera_t * camera, float deltaTime);
status_t camera_move_back(camera_t * camera, float deltaTime);
status_t camera_move_right(camera_t * camera, float deltaTime);

status_t camera_pan(camera_t * camera, vec3 deltaLook, float deltaTime);

status_t camera_update(camera_t * camera, int width, int height);

#ifdef __cplusplus
}
#endif //_cplusplus

#endif //__CAMERA_H__