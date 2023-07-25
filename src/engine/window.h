#ifndef __WINDOW_H__
#define __WINDOW_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>

#define WINDOW_SUCCESS 0
#define WINDOW_FAILURE -1

typedef int status_t;

status_t window_create(GLFWwindow ** window, uint32_t width, uint32_t height, const char * title);

status_t window_close(GLFWwindow ** window);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__WINDOW_H__