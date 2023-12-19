#ifndef __SHADER_HANDLING_H__
#define __SHADER_HANDLING_H__

extern int app_glsl_version;

int get_app_glsl_version(void);
GLuint compile_shader(const GLenum type, const char* filepath);
GLuint create_shader_program(GLFWwindow* window, GLuint vs, GLuint fs);
void free_shader(GLuint* shader);
void free_shader_program(GLuint* shader_program);

#endif

