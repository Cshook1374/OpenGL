#ifndef __MODEL_H__
#define __MODEL_H__

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

#include <GL/glew.h>

#include <stdio.h>
#include <stdlib.h>

#define MODEL_SUCCESS 0
#define MODEL_FAILURE -1

typedef int32_t status_t;

typedef struct {
    uint32_t vaoID;
    uint32_t vertexCount;
} model_t;

status_t model_create(model_t * model, float * positions, size_t positionsLength, float * textures, size_t texturesLength, uint32_t * indices, size_t);

status_t model_render(model_t * model);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__MODEL_H__