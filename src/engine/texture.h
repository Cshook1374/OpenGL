#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

#include <GL/glew.h>

#include <stb/stb_image.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define TEXTURE_SUCCESS 0
#define TEXTURE_FAILURE -1

typedef int32_t status_t;

typedef uint32_t texture_t;

status_t texture_create(texture_t * texture, const char * textureFile);

status_t texture_bind(texture_t * texture);

status_t texture_unbind(void);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__TEXTURE_H__