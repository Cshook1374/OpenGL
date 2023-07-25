#include "shader.h"

status_t shader_create(shader_t * shader, const char * vertexFile, const char* fragmentFile) {
    if (shader == NULL) {
        fprintf(stderr, "shader_t * 'shader' cannot be NULL when being passed to shader_create(...)! ");
        return SHADER_FAILURE;
    }

    if (vertexFile == NULL) {
        fprintf(stderr, "const char * 'vertexFile' cannot be NULL when being passed to shader_create(...)! ");
        return SHADER_FAILURE;
    }

    if (fragmentFile == NULL) {
        fprintf(stderr, "const char * 'fragmentFile' cannot be NULL when being passed to shader_create(...)! ");
        return SHADER_FAILURE;
    }

    uint32_t vertexShaderID;
    status_t vertexCompileStatus = priv_shader_compile(&vertexShaderID, vertexFile, GL_VERTEX_SHADER);
    if (vertexCompileStatus == SHADER_FAILURE) {
        return SHADER_FAILURE;
    }

    uint32_t fragmentShaderID;
    status_t fragmentCompileStatus = priv_shader_compile(&fragmentShaderID, fragmentFile, GL_FRAGMENT_SHADER);
    if (fragmentCompileStatus == SHADER_FAILURE) {
        return SHADER_FAILURE;
    }

    shader->shaderProgramID = glCreateProgram();

    glAttachShader(shader->shaderProgramID, vertexShaderID);
    glAttachShader(shader->shaderProgramID, fragmentShaderID);

    glBindAttribLocation(shader->shaderProgramID, 0, "position");
    glBindAttribLocation(shader->shaderProgramID, 1, "textures");

    glLinkProgram(shader->shaderProgramID);

    shader->location_modelMatrix = glGetUniformLocation(shader->shaderProgramID, "modelMatrix");
    shader->location_viewMatrix = glGetUniformLocation(shader->shaderProgramID, "viewMatrix");
    shader->location_projMatrix = glGetUniformLocation(shader->shaderProgramID, "projMatrix");
    
    int32_t success;
    char infoLog[1024];

    glGetProgramiv(shader->shaderProgramID, GL_LINK_STATUS, &success);
    if (success == GL_FALSE) {
        glGetProgramInfoLog(shader->shaderProgramID, 1024, NULL, infoLog);
        fprintf(stderr, "Failed to link shader program!\n%s\n", infoLog);
        return SHADER_FAILURE;
    }

    return SHADER_SUCCESS;
}

status_t shader_start(shader_t * shader, transform_t * transform, camera_t * camera) {
    if (shader == NULL) {
        fprintf(stderr, "shader_t * 'shader' cannot be NULL when passing to shader_start(...)! ");
        return SHADER_FAILURE;
    }

    glUseProgram(shader->shaderProgramID);

    glUniformMatrix4fv(shader->location_modelMatrix, 1, GL_FALSE, &transform->modelMatrix[0][0]);
    glUniformMatrix4fv(shader->location_viewMatrix, 1, GL_FALSE, &camera->viewMatrix[0][0]);
    glUniformMatrix4fv(shader->location_projMatrix, 1, GL_FALSE, &camera->projMatrix[0][0]);

    return SHADER_SUCCESS;
}

status_t shader_stop() {
    glUseProgram(0);

    return SHADER_SUCCESS;
} 

status_t priv_shader_compile(uint32_t * shaderID, const char * shaderFile, GLenum shaderType) {
    if (shaderID == NULL) {
        fprintf(stderr, "uint32_t * 'shaderID' cannot be NULL when being passed to priv_shader_compile(...)! ");
        return SHADER_FAILURE;
    }

    if (shaderFile == NULL) {
        fprintf(stderr, "const char * 'shaderFile' cannot be NULL when being passed to priv_shader_compile(...)! ");
        return SHADER_FAILURE;
    }

    if ((shaderType != GL_VERTEX_SHADER) && (shaderType != GL_FRAGMENT_SHADER)) {
        fprintf(stderr, "Valid shader types are: GL_VERTEX_SHADER and GL_FRAGMENT_SHADER! ");
        return SHADER_FAILURE;
    }

    *shaderID = glCreateShader(shaderType);

    FILE* file = fopen(shaderFile, "rb");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file %s! %s ", shaderFile, strerror(errno));
        return SHADER_FAILURE;
    }

    status_t errSeek = fseek(file, 0, SEEK_END);
    if (errSeek < 0) {
        fprintf(stderr, "Failed to seek in file %s! %s", shaderFile, strerror(errno));
        return SHADER_FAILURE;
    }

    long length = ftell(file);
    if (length < 0) {
        fprintf(stderr, "File %s's contents has a length of less than zero! ", shaderFile);
        return SHADER_FAILURE;
    }

    errSeek = fseek(file, 0, SEEK_SET);
    if (errSeek < 0) {
        fprintf(stderr, "Failed to seek in file %s! %s", shaderFile, strerror(errno));
        return SHADER_FAILURE;
    }

    char * buffer = malloc(sizeof(char) * length);
    if (buffer == NULL) {
        fprintf(stderr, "Failed to allocate memory for char * 'buffer' in priv_shader_compile(...)! ");
        return SHADER_FAILURE;
    }

    size_t bytesRead = fread(buffer, sizeof(buffer[0]), length, file);
    if (bytesRead != length) {
        fprintf(stderr, "Number of bytes read from file %s does not match length of file %s!\nNumber of bytes read: %zu, Length of %s: %zu ", shaderFile, shaderFile, bytesRead, shaderFile, length);
        return SHADER_FAILURE;
    }

    status_t errClose = fclose(file);
    if (errClose < 0) {
        fprintf(stderr, "Failed to close file %s! %s", shaderFile, strerror(errno));
        return SHADER_FAILURE;
    }

    const char * shaderSource = buffer;

    glShaderSource(*shaderID, 1, &shaderSource, NULL);

    //printf("%s\n", shaderSource);

    free(buffer);
    buffer = NULL;
    shaderSource = NULL;

    glCompileShader(*shaderID);

    int32_t success;
    char infoLog[1024];

    glGetShaderiv(*shaderID, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE) {
        glGetShaderInfoLog(*shaderID, 1024, NULL, infoLog);
        if (shaderType == GL_VERTEX_SHADER) {
            fprintf(stderr, "Failed to compile vertex shader!\n%s\n", infoLog);
            return SHADER_FAILURE;
        }
        if (shaderType == GL_FRAGMENT_SHADER) {
            fprintf(stderr, "Failed to compile fragment shader!\n%s\n", infoLog);
            return SHADER_FAILURE;
        }
    }

    return SHADER_SUCCESS;
}