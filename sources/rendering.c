#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../headers/rendering.h"

/* IMMEDIATE RENDERING MODE ------------------------------------------------ */

void irm_triangle_white(void)
{
    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 1.0, 1.0);
    glVertex3f(0.0, 0.5, 0.0);
    glVertex3f(0.5, -0.5, 0.0);
    glVertex3f(-0.5, -0.5, 0.0);
    glEnd();
    return;
}

void irm_triangle_rgb(void)
{
    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(0.0, 0.5, 0.0);
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0.5, -0.5, 0.0);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(-0.5, -0.5, 0.0);
    glEnd();
    return;
}

void irm_rectangle_bicolor(void)
{
    /* Red */
    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(-0.5, 0.5, 0.0);
    glVertex3f(0.5, -0.5, 0.0);
    glVertex3f(-0.5, -0.5, 0.0);
    glEnd();

    /* Green */
    glBegin(GL_TRIANGLES);
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(-0.5, 0.5, 0.0);
    glVertex3f(0.5, 0.5, 0.0);
    glVertex3f(0.5, -0.5, 0.0);
    glEnd();
    return;
}

void irm_viewport_white(void)
{
    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 1.0, 1.0);
    glVertex3f(-1, 1, 0.0);
    glVertex3f(1, 1, 0.0);
    glVertex3f(1, -1, 0.0);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 1.0, 1.0);
    glVertex3f(-1, 1, 0.0);
    glVertex3f(1, -1, 0.0);
    glVertex3f(-1, -1, 0.0);
    glEnd();
    return;
}

/* SHADERS ----------------------------------------------------------------- */

GLuint create_mesh_vao(const GLfloat points[], const int points_len, 
    const int nbr_vertices, const GLenum usage)
{
    /* Vertex Buffer Object / Vertex Array Object */
    GLuint VBO, VAO;

    /*  ARRAY EXAMPLE:
        ```
        float points[] =
        {
            0.0f,  0.5f,  0.0f,
            0.5f, -0.5f,  0.0f,
            -0.5f, -0.5f,  0.0f
        };
        ```

        USAGE EXAMPLE: `GL_STATIC_DRAW`
    */

    VBO = 0;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, points_len * sizeof(float), points, usage);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, nbr_vertices, GL_FLOAT, GL_FALSE, 0, NULL);

    return VAO;
}

GLuint create_shader_program(const char* vertex_shader, 
    const char* fragment_shader)
{
    /*  EXAMPLES

        const char* vertex_shader =
        "#version 400\n"
        "in vec3 vp;"
        "void main()"
        "{"
        "  gl_Position = vec4(vp, 1.0);"
        "}";

        const char* fragment_shader =
        "#version 400\n"
        "out vec4 frag_colour;"
        "void main()"
        "{"
        "  frag_colour = vec4(0.5, 0.8, 1.0, 1.0);"
        "}";
    */

    GLuint vs, fs, shader_program;

    vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, NULL);
    glCompileShader(vs);

    fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, NULL);
    glCompileShader(fs);

    shader_program = glCreateProgram();
    glAttachShader(shader_program, vs);
    glAttachShader(shader_program, fs);
    glLinkProgram(shader_program);

    /*
        The VS and FS are compiled into the shader program, so there's no use 
        for them anymore. Free them.
    */
    glDeleteShader(vs);
    glDeleteShader(fs);

    return shader_program;
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

void free_shader_program(const GLuint shader_program)
{
    glDeleteProgram(shader_program);
    return;
}

