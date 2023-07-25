#include "model.h"

status_t model_create(model_t * model, float * positions, size_t positionsLength, float * textures, size_t texturesLength, uint32_t * indices, size_t indicesLength) {
    if (model == NULL) {
        fprintf(stderr, "model_t * 'model' cannot be NULL when being passed to model_create(...)! ");
        return MODEL_FAILURE;
    }

    if (positions == NULL) {
        fprintf(stderr, "float * 'positions' cannot be NULL when being passed to model_create(...)! ");
        return MODEL_FAILURE;
    }

    if (textures == NULL) {
        fprintf(stderr, "float * 'textures' cannot be NULL when being passed to model_create(...)! ");
        return MODEL_FAILURE;
    }

    if (indices == NULL) {
        fprintf(stderr, "uint32_t * 'indices' cannot be NULL when being passed to model_create(...)! ");
        return MODEL_FAILURE;
    }

    model->vertexCount = indicesLength;

    glGenVertexArrays(1, &model->vaoID);
    glBindVertexArray(model->vaoID);

    uint32_t vboID;
    glGenBuffers(1, &vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*positions) * positionsLength, positions, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    uint32_t tboID;
    glGenBuffers(1, &tboID);
    glBindBuffer(GL_ARRAY_BUFFER, tboID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*textures) * texturesLength, textures, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    uint32_t eboID;
    glGenBuffers(1, &eboID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(*indices) * indicesLength, indices, GL_STATIC_DRAW);

    glBindVertexArray(0);

    return MODEL_SUCCESS;
}

status_t model_render(model_t * model) {
    if (model == NULL) {
        fprintf(stderr, "NULL model_t * 'model' passed to model_create(...)! ");
        return MODEL_FAILURE;
    }

    glBindVertexArray(model->vaoID);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glDrawElements(GL_TRIANGLES, model->vertexCount, GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    glBindVertexArray(0);

    return 0;
}