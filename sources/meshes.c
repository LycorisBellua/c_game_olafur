#include <GL/glew.h>
#include "../headers/meshes.h"

#define FLOAT_TOLERANCE 0.001f

static const int attr_len = 3;
static const int nbr_attributes = 2;

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

static GLuint create_mesh_vao(const GLfloat vertex_data[], 
    const int vertex_data_len, const int nbr_attributes, const GLenum usage);

void convert_vertex_positions_to_aspect_ratio(const float aspect_ratio)
{
    /*
        Example: Aspect ratio of 16/9 (~= 1.77)
        -----------------------------------------------------------------------
        If the width is larger than the height, the aspect ratio is superior 
        to 1. In this case, only modify the width, that is to say the X axis 
        (index 0 in a vector).

        Example: Aspect ratio of 9/16 (~= 0.56)
        -----------------------------------------------------------------------
        A larger height is expressed by an aspect ratio inferior to 1, which 
        means to only modify the height, and therefore the Y axis (index 1).

        Example: Aspect ratio of 16/16 (= 1)
        -----------------------------------------------------------------------
        A square monitor has an aspect ratio of 1, which means you don't touch 
        the coordinate values. However, given that the aspect ratio is a float 
        you'd need to account for floating point imprecision if you wanted to 
        compare it to an equality. Through trial and error, I have judged that 
        there's a tolerance of 0.001f, meaning that (>= 0.999f && <= 1.001f) 
        is close enough to 1 to be accepted as such.

        However, I have realized that checking for this case is unnecessary. 
        Indeed, using for example 1.002f to modify an axis should go 
        unnoticed on screen (0.5f/1.002f would become 0.499f). So we can just 
        allow it and not bother to compare with 1.
    */

    int i;
    const int index_to_modify = aspect_ratio < 1;
    const float multiplier = index_to_modify ? aspect_ratio : 1 / aspect_ratio;
    int vertex_data_len;

    vertex_data_len = sizeof(triangle)/sizeof(triangle[0]);
    for (i = index_to_modify; i < vertex_data_len; i += attr_len)
    {
        if (i % 2 == index_to_modify)
            triangle[i] *= multiplier;
    }

    vertex_data_len = sizeof(square)/sizeof(square[0]);
    for (i = index_to_modify; i < vertex_data_len; i += attr_len)
    {
        if (i % 2 == index_to_modify)
            square[i] *= multiplier;
    }

    /*
    vertex_data_len = sizeof(viewport)/sizeof(viewport[0]);
    for (i = index_to_modify; i < vertex_data_len; i += attr_len)
    {
        if (i % 2 == index_to_modify)
            viewport[i] *= multiplier;
    }
    */

    return;
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

static GLuint create_mesh_vao(const GLfloat vertex_data[], 
    const int vertex_data_len, const int nbr_attributes, const GLenum usage)
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

