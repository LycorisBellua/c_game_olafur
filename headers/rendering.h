#ifndef __RENDERING_H__
#define __RENDERING_H__

GLuint create_mesh_vao(const GLfloat vertex_data[], const int vertex_data_len, 
    const int nbr_attributes, const GLenum usage);
GLuint compile_shader(const GLenum type, const char* filepath, const int glsl);
GLuint create_shader_program(const GLuint vs, const GLuint fs);

void render_mesh(const GLuint shader_program, const GLuint VAO, 
    const GLenum drawing_mode, const int nbr_vertices);

void free_mesh(const GLuint VAO);
void free_shader(const GLuint shader);
void free_shader_program(const GLuint shader_program);

#endif

