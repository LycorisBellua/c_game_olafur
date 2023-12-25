#ifndef __SHADER_HANDLING_H__
#define __SHADER_HANDLING_H__

extern GLuint world_shader_program;
extern GLuint ui_shader_program;

void set_app_glsl_version(void);
GLuint compile_shader(const GLenum type, const char* filepath);
GLuint create_shader_program(GLFWwindow* window, GLuint vs, GLuint fs);
void free_shader(GLuint* shader);
void free_shader_program(GLuint* shader_program);

#endif

