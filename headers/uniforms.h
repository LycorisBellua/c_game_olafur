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
    UINT
} GlslDatatype;

typedef struct UniformStruct UniformStruct;
typedef void (*UniformCallback)(const UniformStruct* u, const int activate);
struct UniformStruct
{
    GLint loc;
    UniformCallback activate;
    void* data;
};

extern const float pitch;
#define NBR_UNIFORMS 5
extern UniformStruct* uniforms[NBR_UNIFORMS];

void create_uniforms(const GLuint shader_program);
UniformStruct* create_uniform(const GLuint shader_program, const char* name, 
    const UniformCallback activate, ...);
void free_uniforms(void);
void free_uniform(UniformStruct** u);

void activate_uniform_float(const UniformStruct* u, const int activate);
void activate_uniform_int(const UniformStruct* u, const int activate);
void activate_uniform_uint(const UniformStruct* u, const int activate);

void activate_uniform_vec2(const UniformStruct* u, const int activate);
void activate_uniform_vec3(const UniformStruct* u, const int activate);
void activate_uniform_vec4(const UniformStruct* u, const int activate);

void activate_uniform_ivec2(const UniformStruct* u, const int activate);
void activate_uniform_ivec3(const UniformStruct* u, const int activate);
void activate_uniform_ivec4(const UniformStruct* u, const int activate);

void activate_uniform_uvec2(const UniformStruct* u, const int activate);
void activate_uniform_uvec3(const UniformStruct* u, const int activate);
void activate_uniform_uvec4(const UniformStruct* u, const int activate);

void activate_uniform_mat2(const UniformStruct* u, const int activate);
void activate_uniform_mat3(const UniformStruct* u, const int activate);
void activate_uniform_mat4(const UniformStruct* u, const int activate);

#endif

