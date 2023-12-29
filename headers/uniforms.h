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
    ActivateUniformFunction activate;
    void* data;
};

extern const float pitch;
#define NBR_UNIFORMS 5
extern Uniform* uniforms[NBR_UNIFORMS];

#define UNIFORM_MODEL_MATRIX      (uniforms[0])
#define UNIFORM_VIEW_MATRIX       (uniforms[1])
#define UNIFORM_PROJECTION_MATRIX (uniforms[2])
#define UNIFORM_SINGLE_COLOR      (uniforms[3])

void create_uniforms(void);
Uniform* create_uniform(const GLuint id_shader_program, const char* name, 
    const ActivateUniformFunction activate, ...);
void free_uniforms(void);
void free_uniform(Uniform** u);

void activate_uniform_float(const Uniform* u, const int activate);
void activate_uniform_int(const Uniform* u, const int activate);
void activate_uniform_uint(const Uniform* u, const int activate);

void activate_uniform_vec2(const Uniform* u, const int activate);
void activate_uniform_vec3(const Uniform* u, const int activate);
void activate_uniform_vec4(const Uniform* u, const int activate);

void activate_uniform_ivec2(const Uniform* u, const int activate);
void activate_uniform_ivec3(const Uniform* u, const int activate);
void activate_uniform_ivec4(const Uniform* u, const int activate);

void activate_uniform_uvec2(const Uniform* u, const int activate);
void activate_uniform_uvec3(const Uniform* u, const int activate);
void activate_uniform_uvec4(const Uniform* u, const int activate);

void activate_uniform_mat2(const Uniform* u, const int activate);
void activate_uniform_mat3(const Uniform* u, const int activate);
void activate_uniform_mat4(const Uniform* u, const int activate);

#endif

