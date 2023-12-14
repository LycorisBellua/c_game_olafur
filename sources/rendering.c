#include <stdio.h>
#include <stdlib.h>
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

static char* read_file(const char* filepath)
{
    char *ptr = 0;
    long file_length;
    size_t items_read;
    FILE* file;

    file = fopen(filepath, "r");
    if (!file)
    {
        fprintf(stderr, "ERROR: Couldn't open file from \"%s\".", filepath);
        return 0;
    }

    fseek(file, 0, SEEK_END);
    file_length = ftell(file);
    fseek(file, 0, SEEK_SET);
    if (file_length < 0)
    {
        fclose(file);
        fprintf(stderr, "ERROR: Could open but not read file from \"%s\".\n", 
            filepath);
        return 0;
    }

    ptr = malloc((file_length + 1) * sizeof(char));
    if (!ptr)
    {
        fprintf(stderr, "ERROR: Couldn't allocate memory to store content " 
            "from \"%s\".\n", filepath);
        fclose(file);
        return 0;
    }

    items_read = fread(ptr, sizeof(char), file_length, file);
    if (items_read != (size_t)file_length)
    {
        fclose(file);
        free(ptr);
        fprintf(stderr, "ERROR: Could open but not read file from \"%s\".\n", 
            filepath);
        return 0;
    }
    ptr[file_length] = 0;

    fclose(file);
    return ptr;
}

GLuint compile_shader(const GLenum type, const char* filepath)
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

