#include <GL/glew.h>
#include "../headers/meshes.h"

static const int nbr_attributes = 2;
static const int attr_len = 3;

/*
static float triangle_nocolor[] =
{
     0.0f,  0.5f,  0.0f, 
     0.5f, -0.5f,  0.0f, 
    -0.5f, -0.5f,  0.0f,
};
*/

static float triangle[] =
{
     0.0f,  0.5f,  0.0f,    1.0f, 0.0f, 0.0f, 
     0.5f, -0.5f,  0.0f,    0.0f, 1.0f, 0.0f, 
    -0.5f, -0.5f,  0.0f,    0.0f, 0.0f, 1.0f, 
};

static float square[] =
{
    /*
        .
        .  .
    */
    -0.5f,  0.5f,  0.0f,    1.0f, 0.0f, 0.0f, 
     0.5f, -0.5f,  0.0f,    0.0f, 1.0f, 0.0f, 
    -0.5f, -0.5f,  0.0f,    0.0f, 0.0f, 1.0f, 

    /*
        .  .
           .
    */
    -0.5f,  0.5f,  0.0f,    1.0f, 0.0f, 0.0f, 
     0.5f,  0.5f,  0.0f,    0.0f, 1.0f, 0.0f, 
     0.5f, -0.5f,  0.0f,    0.0f, 0.0f, 1.0f, 
};

static float viewport[] =
{
    -1.0f,  1.0f,  0.0f,    1.0f, 1.0f, 1.0f, 
     1.0f,  1.0f,  0.0f,    1.0f, 1.0f, 1.0f, 
     1.0f, -1.0f,  0.0f,    1.0f, 1.0f, 1.0f, 

    -1.0f,  1.0f,  0.0f,    1.0f, 1.0f, 1.0f, 
     1.0f, -1.0f,  0.0f,    1.0f, 1.0f, 1.0f, 
    -1.0f, -1.0f,  0.0f,    1.0f, 1.0f, 1.0f, 
};

static GLuint create_mesh_vao(const GLfloat vertex_data[], const int vertex_data_len, 
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

        - `attr_len` being 3 means vertex_data is organized in vec3.
        - `nbr_attributes * attr_len * sizeof(float)` is the stride, aka how many 
        bytes a vertex is made of in total.
        - Last arg is, in the stride, a pointer to the attribute.
    */
    for (i = 0; i < nbr_attributes; ++i)
    {
        glEnableVertexArrayAttrib(VAO, i);
        glVertexAttribPointer(i, attr_len, GL_FLOAT, GL_FALSE, 
            attr_len * nbr_attributes * sizeof(float), 
            (const void*)(attr_len * i * sizeof(float)));
    }

    /* TODO: Error handling for VAO? */

    return VAO;
}

GLuint create_mesh(const MeshShape shape, int* nbr_vertices)
{
    GLuint VAO = 0;
    float* vertex_data;
    int vertex_data_len;

    if (shape == MESH_TRIANGLE)
    {
        vertex_data = triangle;
        vertex_data_len = sizeof(triangle)/sizeof(triangle[0]);
    }
    else if (shape == MESH_SQUARE)
    {
        vertex_data = square;
        vertex_data_len = sizeof(square)/sizeof(square[0]);
    }
    else if (shape == MESH_VIEWPORT)
    {
        vertex_data = viewport;
        vertex_data_len = sizeof(viewport)/sizeof(viewport[0]);
    }
    else
        return 0;

    VAO = create_mesh_vao(vertex_data, vertex_data_len, nbr_attributes, 
        GL_STATIC_DRAW);

    if (!VAO)
    {
        /* TODO: Error handling */
    }

    *nbr_vertices = vertex_data_len / attr_len / nbr_attributes;
    return VAO;
}

void free_mesh(GLuint* VAO)
{
    int i = 0;
    GLint len = 0;
    GLuint vbo = 0;

    /* Free the VBOs related to the VAO */
    glBindVertexArray(*VAO);
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &len);
    for (i = 0; i < len; ++i)
    {
        vbo = 0;
        glGetVertexAttribIuiv(i, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &vbo);
        if (vbo)
            glDeleteBuffers(1, &vbo);
    }

    /* Free the VAO */
    glDeleteVertexArrays(1, VAO);

    /* Nullify the reference to avoid a double free */
    *VAO = 0;
    return;
}

