#ifndef __SHADER_HANDLING_H__
#define __SHADER_HANDLING_H__

#ifndef __GLEW_H__
#define __GLEW_H__
#include <GL/glew.h>
#endif

typedef struct ShaderProgram ShaderProgram;
typedef void (*ShaderProgramFunction)(const ShaderProgram* instance);

struct ShaderProgram
{
    GLuint id;
    ShaderProgramFunction use;
};

extern ShaderProgram* shader_program_world;
extern ShaderProgram* shader_program_ui;

int create_shader_programs(void);
void free_shader_programs(void);

#endif

