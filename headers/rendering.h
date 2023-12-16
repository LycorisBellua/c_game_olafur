#ifndef __RENDERING_H__
#define __RENDERING_H__

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

/* TODO: Be careful, UniformStruct can currently only handle floats. */
typedef struct
{
    GLint loc;
    UniformType type;
    float x, y, z, w;
} UniformStruct;

GLuint create_mesh_vao(const GLfloat vertex_data[], const int vertex_data_len, 
    const int nbr_attributes, const GLenum usage);
GLuint compile_shader(const GLenum type, const char* filepath, const int glsl);
GLuint create_shader_program(const GLuint vs, const GLuint fs);

UniformStruct init_uniform(const GLuint shader_program, const char* name, 
    const UniformType type, const float x, const float y, const float z, 
    const float w);
void activate_uniform(const UniformStruct* u);
void deactivate_uniform(const UniformStruct* u);

void render_mesh(const GLuint shader_program, const GLuint VAO, 
    const GLenum drawing_mode, const int nbr_vertices);

void free_mesh(const GLuint VAO);
void free_shader(const GLuint shader);
void free_shader_program(const GLuint shader_program);

#endif

