#ifndef __MESHES_H__
#define __MESHES_H__

#ifndef __STDIO_H__
#define __STDIO_H__
#include <stdio.h>
#endif

#ifndef __STDLIB_H__
#define __STDLIB_H__
#include <stdlib.h>
#endif

#ifndef __GLEW_H__
#define __GLEW_H__
#include <GL/glew.h>
#endif

typedef enum
{
    SHAPE_VIEWPORT = 1
} MeshShape;

typedef struct
{
    MeshShape shape;
    GLenum usage;
    GLuint VAO;
    GLuint EBO;

    GLfloat* vertex_data;
    int vertex_data_len;
    int nbr_attributes;
    int attr_len;

    GLuint* indices;
    int indices_len;
} Mesh;

#define NBR_MESHES 2
extern Mesh* meshes[NBR_MESHES];
#define MESH_VIEWPORT (meshes[0])

void create_meshes(void);
void render_mesh(const Mesh* mesh, const GLenum drawing_mode);
void free_meshes(void);

#endif

