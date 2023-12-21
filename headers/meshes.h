#ifndef __MESHES_H__
#define __MESHES_H__

typedef enum
{
    MESH_POINT = 1,
    MESH_TRIANGLE,
    MESH_SQUARE,
    MESH_VIEWPORT
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

void convert_vertex_positions_to_aspect_ratio(const float aspect_ratio);
MeshStruct* create_mesh(const MeshShape shape);
void free_mesh(MeshStruct* mesh);

#endif

