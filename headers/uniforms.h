#ifndef __UNIFORMS_H__
#define __UNIFORMS_H__

/*
typedef enum
{
    UNIFORM_1F = 1,
    UNIFORM_2F,
    UNIFORM_3F,
    UNIFORM_4F,
    UNIFORM_1I,
    UNIFORM_2I,
    UNIFORM_3I,
    UNIFORM_4I,
    UNIFORM_1UI,
    UNIFORM_2UI,
    UNIFORM_3UI,
    UNIFORM_4UI,
    UNIFORM_1FV,
    UNIFORM_2FV,
    UNIFORM_3FV,
    UNIFORM_4FV,
    UNIFORM_1IV,
    UNIFORM_2IV,
    UNIFORM_3IV,
    UNIFORM_4IV,
    UNIFORM_1UIV,
    UNIFORM_2UIV,
    UNIFORM_3UIV,
    UNIFORM_4UIV,
    UNIFORM_MATRIX_2FV,
    UNIFORM_MATRIX_3FV,
    UNIFORM_MATRIX_4FV,
    UNIFORM_MATRIX_2X3FV,
    UNIFORM_MATRIX_3X2FV,
    UNIFORM_MATRIX_2X4FV,
    UNIFORM_MATRIX_4X2FV,
    UNIFORM_MATRIX_3X4FV,
    UNIFORM_MATRIX_4X3FV
} UniformType;
*/

typedef struct UniformStruct UniformStruct;
typedef void (*UniformCallback)(const UniformStruct* u, const int activate);
/* TODO: Be careful, UniformStruct can currently only handle floats. */
struct UniformStruct
{
    GLint loc;
    UniformCallback activate;
    float x, y, z, w;
};


UniformStruct init_uniform(const GLuint shader_program, const char* name, 
    const UniformCallback activate, 
    const float x, const float y, const float z, const float w); 

void activate_uniform_3f(const UniformStruct* u, const int activate);

#endif

