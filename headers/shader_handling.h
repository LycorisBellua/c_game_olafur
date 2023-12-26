#ifndef __SHADER_HANDLING_H__
#define __SHADER_HANDLING_H__

#ifndef __GLEW_H__
#define __GLEW_H__
#include <GL/glew.h>
#endif

extern GLuint id_shader_program_world;
extern GLuint id_shader_program_ui;

GLuint compile_shader(const GLenum type, const char* filepath);
GLuint create_shader_program(GLuint id_vs, GLuint id_fs);
void free_shader(GLuint* id);
void free_shader_program(GLuint* id);

#endif

