#include "engine/model.h"
#include "engine/shader.h"
#include "engine/texture.h"
#include "engine/window.h"
#include "engine/camera.h"
#include "engine/input.h"
#include <GLFW/glfw3.h>

void mouse_callback(GLFWwindow * window, double xpos, double ypos);

camera_t camera;

int main(int argc, char ** argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <winWidth> <winHeight>\n", argv[0]);
        return -1;
    }

    float deltaTime = 0;
    float lastFrame = 0;

    long width = strtol(argv[1], NULL, 10);
    if (width == LONG_MIN || width == LONG_MAX) {
        fprintf(stderr, "Failed to convert %s to an integer! : [%s: %d]\n", argv[1], __FILE__, __LINE__ - 2);
        return -1;
    }

    long height = strtol(argv[2], NULL, 10);
    if (height == LONG_MIN || height == LONG_MAX) {
        fprintf(stderr, "Failed to convert %s to an integer! : [%s: %d]\n", argv[2], __FILE__, __LINE__ - 2);
        return -1;
    }

    GLFWwindow * window;

    status_t winCreate = window_create(&window, width, height, "OpenGL Rendering Port");
    if (winCreate == WINDOW_FAILURE) {
        fprintf(stderr, ": [%s: %d]\n", __FILE__, __LINE__ - 2);
        return -1;
    }

    float positions[] = {-0.5, 0.5, 0, -0.5, -0.5, 0, 0.5, -0.5, 0, 0.5, 0.5, 0};
    size_t positionsLength = sizeof(positions) / sizeof(positions[0]);

    float textures[] = {0, 0, 0, 1, 1, 1, 1, 0};
    size_t texturesLength = sizeof(textures) / sizeof(textures[0]);

    uint32_t indices[] = {0, 1, 3, 3, 1, 2};
    size_t indicesLength = sizeof(indices) / sizeof(indices[0]);

    model_t quad;
    status_t modelCreation = model_create(&quad, positions, positionsLength, textures, texturesLength, indices, indicesLength);
    if (modelCreation == MODEL_FAILURE) {
        fprintf(stderr, ": [%s: %d]\n", __FILE__, __LINE__ - 2);
        return -1;
    }

    transform_t transform;
    status_t transformCreation = transform_create(&transform, (vec3){0, 0, -1}, (vec3){0, 0, 0}, (vec3){1, 1, 1});
    if (transformCreation == TRANSFORM_FAILURE) {
        fprintf(stderr, ": [%s: %d]\n", __FILE__, __LINE__ - 2);
        return -1;
    }

    shader_t shader;
    status_t shaderCreation = shader_create(&shader, "src/shaders/vertexShader.glsl", "src/shaders/fragmentShader.glsl");
    if (shaderCreation == SHADER_FAILURE) {
        fprintf(stderr, ": [%s: %d]\n", __FILE__, __LINE__ - 2);
        return -1;
    }

    texture_t texture;
    status_t textureCreation = texture_create(&texture, "res/textures/grass.jpeg");
    if (textureCreation == TEXTURE_FAILURE) {
        fprintf(stderr, ": [%s: %d]\n", __FILE__, __LINE__ - 2);
        return -1;
    }

    status_t cameraCreation = camera_create(&camera, 60);
    if (cameraCreation == CAMERA_FAILURE) {
        fprintf(stderr, ": [%s: %d]\n", __FILE__, __LINE__ - 2);
        return -1;
    }

    status_t renderStatus;
    status_t shaderStatus;
    status_t textureStatus;
    status_t cameraStatus;
    status_t inputstatus;

    float currentTime = 0;

    float yaw = -90;
    float pitch = 0;

    while (glfwWindowShouldClose(window) == GLFW_FALSE) {
        currentTime = glfwGetTime();
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.8, 0.3, 0.2, 1.0);

        inputstatus = input_keyboard(window, &camera, deltaTime);
        if (inputstatus == INPUT_FAILURE) {
            fprintf(stderr, ": [%s: %d]\n", __FILE__, __LINE__ - 2);
            return -1;
        }

        shaderStatus = shader_start(&shader, &transform, &camera);
        if (shaderStatus == SHADER_FAILURE) {
            fprintf(stderr, ": [%s: %d]\n", __FILE__, __LINE__ - 2);
            return -1;
        }

        cameraStatus = camera_update(&camera, width, height);
        if (cameraStatus == CAMERA_FAILURE) {
            fprintf(stderr, ": [%s: %d]\n", __FILE__, __LINE__ - 2);
            return -1;
        }

        textureStatus = texture_bind(&texture);
        if (textureStatus == TEXTURE_FAILURE) {
            fprintf(stderr, ": [%s: %d]\n", __FILE__, __LINE__ - 2);
            return -1;
        }

        renderStatus = model_render(&quad);
        if (renderStatus == MODEL_FAILURE) {
            fprintf(stderr, "[%s: %d]\n", __FILE__, __LINE__ - 2);
            return -1;
        }

        textureStatus = texture_unbind();
        if (textureStatus == TEXTURE_FAILURE) {
            fprintf(stderr, ": [%s: %d]\n", __FILE__, __LINE__ - 2);
            return -1;
        }

        shaderStatus = shader_stop();
        if (shaderStatus == SHADER_FAILURE) {
            fprintf(stderr, ": [%s: %d]\n", __FILE__, __LINE__ - 2);
            return -1;
        }

        glfwSwapBuffers(window);
        glfwSwapInterval(1);
        glfwPollEvents();
    }

    window_close(&window);

    glfwTerminate();

    return 0;
}

void mouse_callback(GLFWwindow * window, double xpos, double ypos) {
    if (window == NULL) {
        fprintf(stderr, "GLFWwindow * 'window' cannot be NULL when being passed to mouse_callback(...)! ");
        exit(-1);
    }

    bool firstMouse = true;
    double lastX;
    double lastY;
    float yaw;
    float pitch;

    if (firstMouse == true) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 1.0;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 90) {
        pitch = 90;
    }
    if (pitch < -90) {
        pitch = 90;
    }

    vec3 front;
    front[0] = cos(glm_rad(yaw)) * cos(glm_rad(pitch));
    front[1] = sin(glm_rad(pitch));
    front[2] = sin(glm_rad(yaw)) * cos(glm_rad(pitch));
    glm_normalize(front);

    glm_vec3_copy(camera.front, front);
}