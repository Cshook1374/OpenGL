#ifndef __SHADER_H__
#define __SHADER_H__

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

#include <GL/glew.h>

#include "transform.h"

#include "camera.h"

#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <errno.h>

#define SHADER_SUCCESS 0
#define SHADER_FAILURE -1

typedef int32_t status_t;

typedef struct {
    uint32_t shaderProgramID;
    uint32_t location_modelMatrix;
    uint32_t location_viewMatrix;
    uint32_t location_projMatrix;
} shader_t;

status_t shader_create(shader_t * shader, const char * vertexFile, const char* fragmentFile);

status_t shader_start(shader_t * shader, transform_t * transform, camera_t * camera);

status_t shader_stop();

status_t priv_shader_compile(uint32_t * shaderID, const char * shaderFile, GLenum shaderType);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__SHADER_H__