#include "input.h"

status_t input_keyboard(GLFWwindow * window, camera_t * camera, float deltaTime) {
    status_t cameraMoveStatus;
    status_t cameraPanStatus;

    bool firstMouse = true;
    
    if (window == NULL) {
        fprintf(stderr, "GLFWwindow * 'window' cannot be NULL when being passed to input_keyboard(...)! ");
        return INPUT_FAILURE;
    }

    if (glfwGetKey(window, GLFW_KEY_F10) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraMoveStatus = camera_move_front(camera, deltaTime);
        if (cameraMoveStatus == CAMERA_FAILURE) {
            fprintf(stderr, ": [%s: %d]\n",__FILE__, __LINE__ - 2);
            return INPUT_FAILURE;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cameraMoveStatus = camera_move_left(camera, deltaTime);
        if (cameraMoveStatus == CAMERA_FAILURE) {
            fprintf(stderr, ": [%s: %d]\n",__FILE__, __LINE__ - 2);
            return INPUT_FAILURE;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraMoveStatus = camera_move_back(camera, deltaTime);
        if (cameraMoveStatus == CAMERA_FAILURE) {
            fprintf(stderr, ": [%s: %d]\n",__FILE__, __LINE__ - 2);
            return INPUT_FAILURE;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraMoveStatus = camera_move_right(camera, deltaTime);
        if (cameraMoveStatus == CAMERA_FAILURE) {
            fprintf(stderr, ": [%s: %d]\n",__FILE__, __LINE__ - 2);
            return INPUT_FAILURE;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        cameraPanStatus = camera_pan(camera, (vec3){-camera->panSpeed}, deltaTime);
        if (cameraPanStatus == CAMERA_FAILURE) {
            fprintf(stderr, ": [%s: %d]\n", __FILE__, __LINE__ - 2);
            return -1;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        cameraPanStatus = camera_pan(camera, (vec3){camera->panSpeed}, deltaTime);
        if (cameraPanStatus == CAMERA_FAILURE) {
            fprintf(stderr, ": [%s: %d]\n", __FILE__, __LINE__ - 2);
            return -1;
        }
    }

    return INPUT_SUCCESS;
}