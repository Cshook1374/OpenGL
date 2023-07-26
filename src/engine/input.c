#include "input.h"
#include "camera.h"
#include <GLFW/glfw3.h>
#include <cglm/util.h>

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

    return INPUT_SUCCESS;
}

status_t input_mouse(GLFWwindow * window, camera_t * camera, float deltaTime) {
    if (window == NULL) {
        fprintf(stderr, "GLFWwindow * 'window' cannot be NULL when being passed to input_mouse(...)! ");
        return INPUT_FAILURE;
    }

    glfwGetCursorPos(window, &camera->currentX, &camera->currentY);

    float xoffset = camera->currentX - camera->lastX;
    float yoffset = camera->lastY - camera->currentY;
    camera->lastX = camera->currentX;
    camera->lastY = camera->currentY;

    xoffset *= camera->lookSpeed;
    yoffset *= camera->lookSpeed;

    float newYaw = camera->yaw + xoffset;
    float newPitch = camera->pitch + yoffset;

    camera_look(camera, newYaw, newPitch, deltaTime);

    return INPUT_SUCCESS;
}