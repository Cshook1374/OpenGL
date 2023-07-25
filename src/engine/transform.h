#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

#include <cglm/cglm.h>

#include <stdio.h>
#include <stdlib.h>

#define TRANSFORM_SUCCESS 0
#define TRANSFORM_FAILURE -1

typedef int32_t status_t;

typedef struct {
    vec3 position;
    vec3 rotation;
    vec3 scale;
    mat4 modelMatrix;
} transform_t;

status_t transform_create(transform_t * transform, vec3 position, vec3 rotation, vec3 scale);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__TRANSFORM_H__