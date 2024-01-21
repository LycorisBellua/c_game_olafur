#ifndef __SHADER_PROGRAM_H__
#define __SHADER_PROGRAM_H__

#ifndef __GLEW_H__
#define __GLEW_H__
#include <GL/glew.h>
#endif

extern GLuint shader_program;

int create_shader_program(void);
void free_shader_program(void);

#endif

