#ifndef __MESHES_H__
#define __MESHES_H__

typedef enum
{
    MESH_TRIANGLE = 1,
    MESH_SQUARE,
    MESH_VIEWPORT
} MeshShape;

void convert_vertex_positions_to_aspect_ratio(const float aspect_ratio);
GLuint create_mesh(const MeshShape shape, int* nbr_vertices);
void free_mesh(GLuint* VAO);

#endif

