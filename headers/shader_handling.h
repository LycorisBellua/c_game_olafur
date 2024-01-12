#ifndef __SHADER_HANDLING_H__
#define __SHADER_HANDLING_H__

#ifndef __GLEW_H__
#define __GLEW_H__
#include <GL/glew.h>
#endif

typedef struct
{
    GLuint id;
} ShaderProgram;

extern ShaderProgram* shader_program_default;

int create_shader_programs(void);
void use_shader_program(const ShaderProgram* instance);
void free_shader_programs(void);

#endif

