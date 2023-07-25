#ifndef __INPUT_H__
#define __INPUT_H__

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

#include <GLFW/glfw3.h>

#include "camera.h"

#include <stdio.h>
#include <stdlib.h>

#define INPUT_SUCCESS 0
#define INPUT_FAILURE -1

typedef int32_t status_t;

status_t input_keyboard(GLFWwindow * window, camera_t * camera, float deltaTime);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__INPUT_H__