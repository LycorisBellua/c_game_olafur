#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../headers/rendering.h"
#include "../headers/file_handling.h"

GLuint create_mesh_vao(const GLfloat vertex_data[], const int vertex_data_len, 
    const int nbr_attributes, const GLenum usage)
{
    int i;
    /* Vertex Buffer Object / Vertex Array Object */
    GLuint VBO, VAO;

    /*  ARRAY EXAMPLE WITH TWO ATTRIBUTES:
        ```
        float vertex_data[] =
        {
             // Position             // Color
             0.0f,  0.5f,  0.0f,     0.5f, 0.8f, 1.0f, 
             0.5f, -0.5f,  0.0f,     0.5f, 0.8f, 1.0f, 
            -0.5f, -0.5f,  0.0f,     0.5f, 0.8f, 1.0f, 
        };
        ```

        USAGE EXAMPLE: `GL_STATIC_DRAW`
    */

    VBO = 0;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertex_data_len * sizeof(float), 
        vertex_data, usage);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    /*
        Set up attributes. For example, the position attribute is represented 
        by layout(location = 0) in the vertex shader, then 
        layout(location = 1) is the color attribute.

        - The 3 is the args is because our attributes are organized in vec3.
        - `nbr_attributes * 3 * sizeof(float)` is the stride, aka how many 
        bytes a vertex is made of in total.
        - Last arg is, in the stride, a pointer to the attribute.
    */
    for (i = 0; i < nbr_attributes; ++i)
    {
        glEnableVertexArrayAttrib(VAO, i);
        glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, 3 * nbr_attributes * sizeof(float), (const void*)(3 * i * sizeof(float)));
    }

    return VAO;
}

static void set_proper_glsl_version(char* ptr_shader, int glsl)
{
    int i;
    char new_digits[3];

    /* The first line of a shader is something like: "#version 400\n" */

    new_digits[2] = glsl % 10 + '0';
    glsl /= 10;
    new_digits[1] = glsl % 10 + '0';
    glsl /= 10;
    new_digits[0] = glsl % 10 + '0';
    glsl /= 10;

    for (i = 0; ptr_shader[i]; ++i)
    {
        if (ptr_shader[i] >= '0' && ptr_shader[i] <= '9')
        {
            ptr_shader[i] = new_digits[0];
            ptr_shader[i + 1] = new_digits[1];
            ptr_shader[i + 2] = new_digits[2];
            break;
        }
    }
    return;
}

GLuint compile_shader(const GLenum type, const char* filepath, const int glsl)
{
    GLuint shader;
    char* ptr = read_file(filepath);

    if (!ptr)
        return 0;
    else if (type != GL_VERTEX_SHADER && type != GL_FRAGMENT_SHADER)
    {
        free(ptr);
        return 0;
    }

    set_proper_glsl_version(ptr, glsl);

    shader = glCreateShader(type);
    glShaderSource(shader, 1, (char const* const*)&ptr, NULL);
    glCompileShader(shader);
    free(ptr);
    return shader;
}

GLuint create_shader_program(const GLuint vs, const GLuint fs)
{
    GLuint shader_program;

    if (!vs || !fs)
        return 0;

    shader_program = glCreateProgram();
    glAttachShader(shader_program, vs);
    glAttachShader(shader_program, fs);
    glLinkProgram(shader_program);
    return shader_program;
}

void activate_uniform_3f(const UniformStruct* u, const int activate)
{
    if (activate)
        glUniform3f(u->loc, u->x, u->y, u->z);
    else
        glUniform3f(u->loc, 0, 0, 0);
    return;
}

UniformStruct init_uniform(const GLuint shader_program, const char* name, 
    const float x, const float y, const float z, const float w, 
    UniformCallback activate)
{
    UniformStruct u;
    u.loc = glGetUniformLocation(shader_program, name);
    u.x = x;
    u.y = y;
    u.z = z;
    u.w = w;
    u.activate = activate;
    return u;
}

void render_mesh(const GLuint shader_program, const GLuint VAO, 
    const GLenum drawing_mode, const int nbr_vertices)
{
    /* Drawing mode example: GL_TRIANGLES */
    int start_index = 0;

    glUseProgram(shader_program);
    glBindVertexArray(VAO);
    glDrawArrays(drawing_mode, start_index, nbr_vertices);
    return;
}

void free_mesh(const GLuint VAO)
{
    int i = 0;
    GLint len = 0;
    GLuint vbo = 0;

    /* Free the VBOs related to the VAO */
    glBindVertexArray(VAO);
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &len);
    for (i = 0; i < len; ++i)
    {
        vbo = 0;
        glGetVertexAttribIuiv(i, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &vbo);
        if (vbo)
            glDeleteBuffers(1, &vbo);
    }

    /* Free the VAO */
    glDeleteVertexArrays(1, &VAO);
    return;
}

void free_shader(const GLuint shader)
{
    glDeleteShader(shader);
    return;
}

void free_shader_program(const GLuint shader_program)
{
    glDeleteProgram(shader_program);
    return;
}

