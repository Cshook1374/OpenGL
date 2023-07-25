#define STB_IMAGE_IMPLEMENTATION

#include "texture.h"

status_t texture_create(texture_t * texture, const char * textureFile) {
    if (texture == NULL) {
        fprintf(stderr, "texture_t * 'texture' cannot be NULL when being passed to texture_create(...)! ");
        return TEXTURE_FAILURE;
    }

    if (textureFile == NULL) {
        fprintf(stderr, "const char * 'textureFile' cannot be NULL when being passed to texture_create(...)! ");
        return TEXTURE_FAILURE;
    }

    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width, height, nrChannels;
    unsigned char *data = stbi_load(textureFile, &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        fprintf(stderr, "Failed to open texture file %s! %s", textureFile, strerror(errno));
        return TEXTURE_FAILURE;
    }
    stbi_image_free(data);

    glBindTexture(GL_TEXTURE_2D, 0);

    return TEXTURE_SUCCESS;
}

status_t texture_bind(texture_t * texture) {
    if (texture == NULL) {
        fprintf(stderr, "texture_t * 'texture' cannot be NULL when being passed to texture_bind(...)! ");
        return TEXTURE_FAILURE;
    }

    glBindTexture(GL_TEXTURE_2D, *texture);

    return TEXTURE_SUCCESS;
}

status_t texture_unbind(void) {
    glBindTexture(GL_TEXTURE_2D, 0);

    return TEXTURE_SUCCESS;
}