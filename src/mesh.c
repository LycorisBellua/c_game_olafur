#include "../hdr/mesh.h"
#include "../hdr/gl_functions.h"

static GLuint VBO; /* Vertex Buffer Object */
static GLuint VAO; /* Vertex Array Object */
static GLuint EBO; /* Element Buffer Object */

static GLfloat vertex_data[] =
{
     /* Pos */       /* Tex Coord */
    -1.0f, -1.0f,    0.0f, 0.0f, 
     1.0f, -1.0f,    1.0f, 0.0f, 
     1.0f,  1.0f,    1.0f, 1.0f, 
    -1.0f,  1.0f,    0.0f, 1.0f, 
};
static GLuint indices[] = 
{
    0, 1, 2, 
    0, 2, 3
};

void create_mesh(void)
{
    int i;
    const int attr_nbr = 2;
    const int attr_len = 2;

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, 
        GL_STATIC_DRAW);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, 
        GL_STATIC_DRAW);

    /*
        Set up attributes. For example, the position attribute is represented 
        by layout(location = 0) in the vertex shader, then 
        layout(location = 1) is the texture coordinate attribute.

        - `attr_nbr` is the number of attributes (pos and tex coord).
        - `attr_len` being 2 means vertex_data is organized in vec2.
        - `attr_nbr * attr_len * sizeof(GLfloat)` is the stride, aka how 
        many bytes a vertex is made of in total.
        - Last arg is, in the stride, a pointer to the attribute.
    */
    for (i = 0; i < attr_nbr; ++i)
    {
        glEnableVertexArrayAttrib(VAO, i);
        glVertexAttribPointer(i, attr_nbr, GL_FLOAT, GL_FALSE, 
            attr_nbr * attr_len * sizeof(GLfloat), 
            (const void*)(attr_len * i * sizeof(GLfloat)));
    }
    return;
}

void render_mesh(void)
{
    /* 6 is the length of the `indices` array */
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); 
    return;
}

void free_mesh(void)
{
    int i = 0;
    GLint len = 0;

    /* Free the EBO */
    glDeleteBuffers(1, &EBO);

    /* Free the VBOs related to the VAO */
    glBindVertexArray(VAO);
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &len);
    for (i = 0; i < len; ++i)
    {
        VBO = 0;
        glGetVertexAttribIuiv(i, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &VBO);
        if (VBO)
            glDeleteBuffers(1, &VBO);
    }

    /* Free the VAO */
    glDeleteVertexArrays(1, &VAO);
    return;
}

