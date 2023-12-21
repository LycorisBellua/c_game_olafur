#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include "../headers/meshes.h"

#define FLOAT_TOLERANCE 0.001f

static const int attr_len = 3;

/*
static GLfloat triangle_nocolor[] =
{
     0.0f,  0.5f,  0.0f, 
     0.5f, -0.5f,  0.0f, 
    -0.5f, -0.5f,  0.0f,
};
static GLuint triangle_nocolor_indices[] = { 0, 1, 2 };
*/

static GLfloat point[] = 
{
    0.0f, 0.0f, 0.0f,       1.0f, 0.0f, 0.0f,
};
static GLuint point_indices[] = { 0 };

static GLfloat triangle[] =
{
     0.0f,  0.5f,  0.0f,    1.0f, 0.0f, 0.0f, 
     0.5f, -0.5f,  0.0f,    0.0f, 1.0f, 0.0f, 
    -0.5f, -0.5f,  0.0f,    0.0f, 0.0f, 1.0f, 
};
static GLuint triangle_indices[] = { 0, 1, 2 };

static GLfloat square[] =
{
    /*
        .   + . .   0, 1, 2,
        . .     .   0, 3, 1
    */
    -0.5f,  0.5f,  0.0f,    1.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.0f,    0.0f, 1.0f, 0.0f, 
    -0.5f, -0.5f,  0.0f,    0.0f, 0.0f, 1.0f, 
     0.5f,  0.5f,  0.0f,    0.0f, 1.0f, 0.0f, 
};
static GLuint square_indices[] = 
{
    0, 1, 2,
    0, 3, 1
};

static GLfloat viewport[] =
{
    -1.0f,  1.0f,  0.0f,    1.0f, 1.0f, 1.0f, 
     1.0f,  1.0f,  0.0f,    1.0f, 1.0f, 1.0f, 
     1.0f, -1.0f,  0.0f,    1.0f, 1.0f, 1.0f, 
    -1.0f, -1.0f,  0.0f,    1.0f, 1.0f, 1.0f, 
};
static GLuint viewport_indices[] = 
{
    0, 1, 2, 
    0, 2, 3
};

static void create_mesh_buffer_objects(MeshStruct* mesh);

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

MeshStruct* create_mesh(const MeshShape shape)
{
    MeshStruct* mesh = 0;

    mesh = malloc(sizeof(MeshStruct));
    if (!mesh)
    {
        fprintf(stderr, "ERROR: Couldn't allocate memory for mesh.\n");
        return 0;
    }

    mesh->shape = shape;
    mesh->usage = GL_STATIC_DRAW;
    mesh->VAO = 0;
    mesh->EBO = 0;
    mesh->nbr_attributes = 2;

    if (shape == MESH_POINT)
    {
        mesh->vertex_data = point;
        mesh->vertex_data_len = sizeof(point)/sizeof(point[0]);
        mesh->indices = point_indices;
        mesh->indices_len = sizeof(point_indices)
            /sizeof(point_indices[0]);
    }
    else if (shape == MESH_TRIANGLE)
    {
        mesh->vertex_data = triangle;
        mesh->vertex_data_len = sizeof(triangle)/sizeof(triangle[0]);
        mesh->indices = triangle_indices;
        mesh->indices_len = sizeof(triangle_indices)
            /sizeof(triangle_indices[0]);
    }
    else if (shape == MESH_SQUARE)
    {
        mesh->vertex_data = square;
        mesh->vertex_data_len = sizeof(square)/sizeof(square[0]);
        mesh->indices = square_indices;
        mesh->indices_len = sizeof(square_indices)/sizeof(square_indices[0]);
    }
    else if (shape == MESH_VIEWPORT)
    {
        mesh->vertex_data = viewport;
        mesh->vertex_data_len = sizeof(viewport)/sizeof(viewport[0]);
        mesh->indices = viewport_indices;
        mesh->indices_len = sizeof(viewport_indices)
            /sizeof(viewport_indices[0]);
    }
    else
    {
        free(mesh);
        fprintf(stderr, "ERROR: Invalid enum for mesh shape. "
            "Mesh not created.\n");
        return 0;
    }

    create_mesh_buffer_objects(mesh);

    return mesh;
}

void free_mesh(MeshStruct* mesh)
{
    int i = 0;
    GLint len = 0;
    GLuint VBO = 0;

    if (!mesh)
        return;

    /* Free the EBO */
    glDeleteBuffers(1, &mesh->EBO);

    /* Free the VBOs related to the VAO */
    glBindVertexArray(mesh->VAO);
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &len);
    for (i = 0; i < len; ++i)
    {
        VBO = 0;
        glGetVertexAttribIuiv(i, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &VBO);
        if (VBO)
            glDeleteBuffers(1, &VBO);
    }

    /* Free the VAO */
    glDeleteVertexArrays(1, &mesh->VAO);

    /* Free the struct */
    free(mesh);

    /* Nullify the reference to avoid a double free */
    mesh = 0;
    return;
}

static void create_mesh_buffer_objects(MeshStruct* mesh)
{
    int i;
    GLuint VBO; /* Vertex Buffer Object */
    GLuint VAO; /* Vertex Array Object */
    GLuint EBO; /* Element Buffer Object */

    /*  ARRAY EXAMPLE WITH TWO ATTRIBUTES:
        ```
        GLfloat vertex_data[] =
        {
             // Position             // Color
             0.0f,  0.5f,  0.0f,     0.5f, 0.8f, 1.0f, 
             0.5f, -0.5f,  0.0f,     0.5f, 0.8f, 1.0f, 
            -0.5f, -0.5f,  0.0f,     0.5f, 0.8f, 1.0f, 
        };
        ```
    */

    if (!mesh)
        return;

    VBO = 0;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh->vertex_data_len * sizeof(GLfloat), 
        mesh->vertex_data, mesh->usage);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices_len * sizeof(GLuint), 
        mesh->indices, mesh->usage);

    /*
        Set up attributes. For example, the position attribute is represented 
        by layout(location = 0) in the vertex shader, then 
        layout(location = 1) is the color attribute.

        - `attr_len` being 3 means vertex_data is organized in vec3.
        - `nbr_attributes * attr_len * sizeof(GLfloat)` is the stride, aka how 
        many bytes a vertex is made of in total.
        - Last arg is, in the stride, a pointer to the attribute.
    */
    for (i = 0; i < mesh->nbr_attributes; ++i)
    {
        glEnableVertexArrayAttrib(VAO, i);
        glVertexAttribPointer(i, attr_len, GL_FLOAT, GL_FALSE, 
            attr_len * mesh->nbr_attributes * sizeof(GLfloat), 
            (const void*)(attr_len * i * sizeof(GLfloat)));
    }

    /* TODO: Error handling for VBO, VAO or EBO? */

    mesh->VAO = VAO;
    mesh->EBO = EBO;
    return;
}

