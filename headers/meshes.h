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
    SHAPE_POINT = 1,
    SHAPE_TRIANGLE,
    SHAPE_SQUARE,
    SHAPE_VIEWPORT,
    SHAPE_CUBE,
    SHAPE_FLOOR
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

    GLuint* indices;
    int indices_len;
} MeshStruct;

#define NBR_MESHES 7
extern MeshStruct* meshes[NBR_MESHES];

void create_meshes(void);
void convert_vertex_positions_to_aspect_ratio(const float aspect_ratio);
MeshStruct* create_mesh(const MeshShape shape);
void free_meshes(void);
void free_mesh(MeshStruct** mesh);

#endif

