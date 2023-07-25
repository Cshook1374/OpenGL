#include "window.h"

status_t window_create(GLFWwindow ** window, uint32_t width, uint32_t height, const char * title) {
    if (window == NULL) {
        fprintf(stderr, "NULL GLFWwindow ** passed to window_create(...)! ");
        return WINDOW_FAILURE;
    }
    
    if (width <= 0 || height <= 0) {
        fprintf(stderr, "Window cannot have dimensions <= 0! ");
        return WINDOW_FAILURE;
    }

    if (glfwInit() == GLFW_FALSE) {
        fprintf(stderr, "Failed to init GLFW! ");
        return WINDOW_FAILURE;
    }

    *window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (*window == NULL) {
        fprintf(stderr, "Failed to create window! ");
        glfwTerminate();
        return WINDOW_FAILURE;
    }

    glfwMakeContextCurrent(*window);

    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to init GLEW! ");
        glfwTerminate();
        return WINDOW_FAILURE;
    }

    return WINDOW_SUCCESS;
}

status_t window_close(GLFWwindow ** window) {
    if (*window == NULL) {
        fprintf(stderr, "Cannot close a NULL window! ");
        return -1;
    }

    glfwDestroyWindow(*window);

    return WINDOW_SUCCESS;
}