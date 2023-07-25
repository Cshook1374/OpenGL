#include "transform.h"

status_t transform_create(transform_t * transform, vec3 position, vec3 rotation, vec3 scale) {
    if (transform == NULL) {
        fprintf(stderr, "transform_t * 'transform' cannot be NULL when being passed to transform_create(...)! ");
        return TRANSFORM_FAILURE;
    }

    if (position == NULL) {
        fprintf(stderr, "vec3 'position' cannot be NULL when being passed to transform_create(...)! ");
        return TRANSFORM_FAILURE;
    }

    if (rotation == NULL) {
        fprintf(stderr, "vec3 'rotation' cannot be NULL when being passed to transform_create(...)! ");
        return TRANSFORM_FAILURE;
    }

    if (scale == NULL) {
        fprintf(stderr, "vec3 'scale' cannot be NULL when being passed to transform_create(...)! ");
        return TRANSFORM_FAILURE;
    }

    glm_vec3_copy(position, transform->position);
    glm_vec3_copy(rotation, transform->rotation);
    glm_vec3_copy(scale, transform->scale);

    glm_mat4_copy(GLM_MAT4_IDENTITY, transform->modelMatrix);

    glm_translate(transform->modelMatrix, transform->position);
    glm_rotate(transform->modelMatrix, glm_rad(transform->rotation[0]), (vec3){1, 0, 0});
    glm_rotate(transform->modelMatrix, glm_rad(transform->rotation[1]), (vec3){0, 1, 0});
    glm_rotate(transform->modelMatrix, glm_rad(transform->rotation[2]), (vec3){0, 0, 1});
    glm_scale(transform->modelMatrix, transform->scale);

    return TRANSFORM_SUCCESS;
}