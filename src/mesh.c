#include "cimmerian.h"

static GLuint VBO; /* Vertex Buffer Object */
static GLuint VAO; /* Vertex Array Object */
static GLuint EBO; /* Element Buffer Object */

static GLdouble vertex_data[] =
{
     /* Pos */     /* Tex Coord */
    -1.0, -1.0,    0.0, 1.0,
     1.0, -1.0,    1.0, 1.0,
     1.0,  1.0,    1.0, 0.0,
    -1.0,  1.0,    0.0, 0.0,
};
static GLuint indices[] = 
{
    0, 1, 2, 
    0, 2, 3
};

int create_mesh(void)
{
    int i;
    int attr_nbr;
    int attr_len;

    attr_nbr = 2;
    attr_len = 2;

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
        by layout(location = 0) in the vertex shader, then layout(location = 1) 
        is the texture coordinate attribute.

        - `attr_nbr` is the number of attributes (pos and tex coord).
        - `attr_len` being 2 means vertex_data is organized in vec2.
        - `attr_nbr * attr_len * sizeof(GLdouble)` is the stride, aka how many 
        bytes a vertex is made of in total.
        - Last arg is, in the stride, a pointer to the attribute.
    */
    i = 0;
    while (i < attr_nbr)
    {
        glEnableVertexArrayAttrib(VAO, i);
        glVertexAttribPointer(i, attr_nbr, GL_DOUBLE, GL_FALSE, 
            attr_nbr * attr_len * sizeof(GLdouble), 
            (void*)(attr_len * i * sizeof(GLdouble)));
        ++i;
    }
    return 1;
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
    int i;
    GLint len;

    i = 0;
    len = 0;

    /* Free the EBO */
    glDeleteBuffers(1, &EBO);

    /* Free the VBOs related to the VAO */
    glBindVertexArray(VAO);
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &len);
    i = 0;
    while (i < len)
    {
        VBO = 0;
        glGetVertexAttribIuiv(i, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &VBO);
        if (VBO)
            glDeleteBuffers(1, &VBO);
        ++i;
    }

    /* Free the VAO */
    glDeleteVertexArrays(1, &VAO);
    return;
}
