#include "engine/model.h"
#include "engine/shader.h"
#include "engine/texture.h"
#include "engine/window.h"
#include "engine/camera.h"
#include "engine/input.h"

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

    camera_t camera;
    status_t cameraCreation = camera_create(&camera, 60, 0.5, width, height);
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

        inputstatus = input_mouse(window, &camera, deltaTime);
        if (inputstatus == INPUT_FAILURE) {
            fprintf(stderr, ": [%s: %d]\n", __FILE__, __LINE__ - 2);
            return -1;
        }

        printf("Mouse X: %lf, Mouse Y: %lf\n", camera.currentX, camera.currentY);

        shaderStatus = shader_start(&shader, &transform, &camera);
        if (shaderStatus == SHADER_FAILURE) {
            fprintf(stderr, ": [%s: %d]\n", __FILE__, __LINE__ - 2);
            return -1;
        }

        cameraStatus = camera_update(&camera);
        if (cameraStatus == CAMERA_FAILURE) {
            fprintf(stderr, ": [%s: %d]\n", __FILE__, __LINE__ - 2);
            return -1;
        }

        cameraStatus = camera_print(&camera);
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