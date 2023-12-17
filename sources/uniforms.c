#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <GL/glew.h>
#include "../headers/uniforms.h"

UniformStruct* create_uniform(const GLuint shader_program, const char* name, 
    const UniformCallback activate, void* data)
{
    UniformStruct* ptr = malloc(sizeof(UniformStruct));
    if (!ptr)
    {
        fprintf(stderr, "ERROR: Couldn't allocate memory for \"%s\" uniform.", 
            name);
        exit(EXIT_FAILURE);
    }

    ptr->loc = glGetUniformLocation(shader_program, name);
    ptr->activate = activate;
    ptr->data = data;
    return ptr;
}

/*
    GLSL basic datatypes
    - float floatNumber = 3.14;
    - int signedInt = -5;
    - uint unsignedInt = 10u;

    - [actually int] bool isTrue = true;
    - [doubles may not work]
*/

void activate_uniform_float(const UniformStruct* u, const int activate)
{
    if (activate)
        glUniform1f(u->loc, *(float*)u->data);
    else
        glUniform1f(u->loc, 0);
    return;
}

void activate_uniform_int(const UniformStruct* u, const int activate)
{
    if (activate)
        glUniform1i(u->loc, *(int*)u->data);
    else
        glUniform1i(u->loc, 0);
    return;
}

void activate_uniform_uint(const UniformStruct* u, const int activate)
{
    if (activate)
        glUniform1ui(u->loc, *(unsigned int*)u->data);
    else
        glUniform1ui(u->loc, 0);
    return;
}

/*
    GLSL vector datatypes
    - [float] vec2, vec3, vec4
    - [int] ivec2, ivec3, ivec4
    - [uint] uvec2, uvec3, uvec4

    - [bool] bvec2, bvec3, bvec4
    [Apparently GLSL treat them as actual booleans and not integers, 
    but there is no glUniform function to set booleans, so just use 
    integer vectors instead.]
*/

void activate_uniform_vec2(const UniformStruct* u, const int activate)
{
    if (activate)
        glUniform2fv(u->loc, 1, (float*)u->data);
    else
        glUniform2f(u->loc, 0, 0);
    return;
}

void activate_uniform_vec3(const UniformStruct* u, const int activate)
{
    if (activate)
        glUniform3fv(u->loc, 1, (float*)u->data);
    else
        glUniform3f(u->loc, 0, 0, 0);
    return;
}

void activate_uniform_vec4(const UniformStruct* u, const int activate)
{
    if (activate)
        glUniform4fv(u->loc, 1, (float*)u->data);
    else
        glUniform4f(u->loc, 0, 0, 0, 0);
    return;
}

void activate_uniform_ivec2(const UniformStruct* u, const int activate)
{
    if (activate)
        glUniform2iv(u->loc, 1, (int*)u->data);
    else
        glUniform2i(u->loc, 0, 0);
    return;
}

void activate_uniform_ivec3(const UniformStruct* u, const int activate)
{
    if (activate)
        glUniform3iv(u->loc, 1, (int*)u->data);
    else
        glUniform3i(u->loc, 0, 0, 0);
    return;
}

void activate_uniform_ivec4(const UniformStruct* u, const int activate)
{
    if (activate)
        glUniform4iv(u->loc, 1, (int*)u->data);
    else
        glUniform4i(u->loc, 0, 0, 0, 0);
    return;
}

void activate_uniform_uvec2(const UniformStruct* u, const int activate)
{
    if (activate)
        glUniform2uiv(u->loc, 1, (unsigned int*)u->data);
    else
        glUniform2ui(u->loc, 0, 0);
    return;
}

void activate_uniform_uvec3(const UniformStruct* u, const int activate)
{
    if (activate)
        glUniform3uiv(u->loc, 1, (unsigned int*)u->data);
    else
        glUniform3ui(u->loc, 0, 0, 0);
    return;
}

void activate_uniform_uvec4(const UniformStruct* u, const int activate)
{
    if (activate)
        glUniform4uiv(u->loc, 1, (unsigned int*)u->data);
    else
        glUniform4ui(u->loc, 0, 0, 0, 0);
    return;
}

/*
    GLSL matrix datatypes (= multidimensional array with float elements)
    - mat2: 2x2 array
    - mat3: 3x3 array
    - mat4: 4x4 array
*/

/*
    TODO: Figure out the `transpose` parameter (the boolean).
    ---------------------------------------------------------------------------
    - If transpose is set to GL_FALSE, it means that the matrices are supplied 
    in column-major order, where each sequence of four contiguous elements 
    represents a column of the matrix.
    - If transpose is set to GL_TRUE, it means that the matrices are supplied 
    in row-major order, where each sequence of four contiguous elements 
    represents a row of the matrix.
    - The choice of setting transpose to GL_TRUE or GL_FALSE depends on how 
    your matrices are stored or generated. In many cases, matrices are 
    created in row-major order (for example, in code or in some file formats), 
    and you might need to set transpose to GL_TRUE to correctly interpret them 
    in OpenGL. If your matrices are in column-major order, you can set 
    transpose to GL_FALSE or simply use the matrices as they are.
*/

void activate_uniform_mat2(const UniformStruct* u, const int activate)
{
    if (activate)
        glUniformMatrix2fv(u->loc, 1, GL_TRUE, (float*)u->data);
    else
        glUniformMatrix2fv(u->loc, 1, GL_TRUE, 0);
    return;
}

void activate_uniform_mat3(const UniformStruct* u, const int activate)
{
    if (activate)
        glUniformMatrix3fv(u->loc, 1, GL_TRUE, (float*)u->data);
    else
        glUniformMatrix3fv(u->loc, 1, GL_TRUE, 0);
    return;
}

void activate_uniform_mat4(const UniformStruct* u, const int activate)
{
    if (activate)
        glUniformMatrix4fv(u->loc, 1, GL_TRUE, (float*)u->data);
    else
        glUniformMatrix4fv(u->loc, 1, GL_TRUE, 0);
    return;
}

