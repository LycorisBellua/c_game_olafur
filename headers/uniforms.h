#ifndef __UNIFORMS_H__
#define __UNIFORMS_H__

#ifndef __STDIO_H__
#define __STDIO_H__
#include <stdio.h>
#endif

#ifndef __STDLIB_H__
#define __STDLIB_H__
#include <stdlib.h>
#endif

#ifndef __STDARG_H__
#define __STDARG_H__
#include <stdarg.h>
#endif

#ifndef __GLEW_H__
#define __GLEW_H__
#include <GL/glew.h>
#endif

typedef enum
{
    FLOAT = 1,
    INT,
    UINT,
    MATRIX
} GlslDatatype;

typedef struct Uniform Uniform;
typedef void (*ActivateUniformFunction)(const Uniform* instance, 
    const int activate);
struct Uniform
{
    GLint loc;
    int activate_on_create;
    ActivateUniformFunction activate;
    void* data;
};

#define NBR_UNIFORMS 2
extern Uniform* uniforms[NBR_UNIFORMS];
#define UNIFORM_FRAME_TEXTURE (uniforms[0])

void create_uniforms(void);
void free_uniforms(void);

#endif

