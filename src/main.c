#include "engine/model.h"
#include "engine/shader.h"
#include "engine/window.h"

int main(int argc, char ** argv) {
    GLFWwindow * window;

    status_t winCreate = window_create(&window, 800, 600, "OpenGL Rendering Port");
    if (winCreate == WINDOW_FAILURE) {
        fprintf(stderr, ": [%s: %d]\n", __FILE__, __LINE__ - 2);
        return -1;
    }

    float positions[] = {-0.5, 0.5, 0, -0.5, -0.5, 0, 0.5, -0.5, 0, 0.5, 0.5, 0};
    size_t positionsLength = sizeof(positions) / sizeof(positions[0]);

    uint32_t indices[] = {0, 1, 3, 3, 1, 2};
    size_t indicesLength = sizeof(indices) / sizeof(indices[0]);

    model_t quad;
    status_t modelCreation = model_create(&quad, positions, positionsLength, indices, indicesLength);
    if (modelCreation == MODEL_FAILURE) {
        fprintf(stderr, ": [%s: %d]\n", __FILE__, __LINE__ - 2);
        return -1;
    }

    shader_t shader;
    status_t shaderCreation = shader_create(&shader, "src/shaders/vertexShader.glsl", "src/shaders/fragmentShader.glsl");
    if (shaderCreation == SHADER_FAILURE) {
        fprintf(stderr, ": [%s: %d]\n", __FILE__, __LINE__ - 2);
        return -1;
    }

    status_t renderStatus;
    status_t shaderStatus;

    while (glfwWindowShouldClose(window) == GLFW_FALSE) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.8, 0.3, 0.2, 1.0);

        shaderStatus = shader_start(&shader);
        if (shaderStatus == SHADER_FAILURE) {
            fprintf(stderr, ": [%s: %d]\n", __FILE__, __LINE__ - 2);
            return -1;
        }

        renderStatus = model_render(&quad);
        if (renderStatus == MODEL_FAILURE) {
            fprintf(stderr, "[%s: %d]\n", __FILE__, __LINE__ - 2);
            return -1;
        }

        shaderStatus = shader_stop();
        if (shaderStatus == SHADER_FAILURE) {
            fprintf(stderr, ": [%s: %d]\n", __FILE__, __LINE__ - 2);
            return -1;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    window_close(&window);

    glfwTerminate();

    return 0;
}