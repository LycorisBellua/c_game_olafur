#include <GL/glew.h>
#include "../headers/meshes.h"
#include "../headers/rendering.h"

void render_mesh(const MeshStruct* mesh, const GLenum drawing_mode)
{
    if (!mesh)
        return;

    glBindVertexArray(mesh->VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
    glDrawElements(drawing_mode, mesh->indices_len, GL_UNSIGNED_INT, 0); 
    return;
}

