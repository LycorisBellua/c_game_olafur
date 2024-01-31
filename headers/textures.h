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

#ifndef __STRING_H__
#define __STRING_H__
#include <string.h>
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
    int real_width;
    int real_height;
    int thickness;
    int width;
    int height;
    GLubyte* buffer;
} Texture;

#define NBR_TEXTURES 2
extern Texture* textures[NBR_TEXTURES];
#define TEX_MAIN (textures[0])

void create_textures(void);
void use_texture(const Texture* t);
void clear_drawing(Texture* t, GLubyte value);
void save_drawing(const Texture* t);
void free_textures(void);

#endif

