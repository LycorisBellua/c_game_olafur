#ifndef __TEXTURES_H__
#define __TEXTURES_H__

#ifndef __STDIO_H__
#define __STDIO_H__
#include <stdio.h>
#endif

#ifndef __STDLIB_H__
#define __STDLIB_H__
#include <stdlib.h>
#endif

#ifndef __GLEW_H__
#define __GLEW_H__
#include <GL/glew.h>
#endif

#ifndef __GLFW_H__
#define __GLFW_H__
#include <GLFW/glfw3.h>
#endif

typedef struct
{
    GLuint id;
    int width;
    int height;
    unsigned char* buffer;
} Texture;

typedef struct
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
} Color;

#define NBR_TEXTURES 2
extern Texture* textures[NBR_TEXTURES];
#define TEXTURE_GAME (textures[0])

void create_textures(void);
void free_textures(void);

#endif

