#include <GL/glew.h>
#include "../headers/meshes.h"
#include "../headers/rendering.h"
#include "../headers/uniforms.h"

/*
   GL_POINTS

   GL_LINES
   GL_LINE_STRIP
   GL_LINE_LOOP

   GL_TRIANGLES
   GL_TRIANGLE_STRIP
   GL_TRIANGLE_FAN
*/

void render_mesh(const MeshStruct* mesh, const GLenum drawing_mode)
{
    if (!mesh)
        return;

    glBindVertexArray(mesh->VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
    glDrawElements(drawing_mode, mesh->indices_len, GL_UNSIGNED_INT, 0); 
    return;
}

void render_main_menu(void)
{
    /* Triangle & "single_color" uniform */
    uniforms[0]->activate(uniforms[0], 0);
    render_mesh(meshes[2], GL_TRIANGLES);
    return;
}

void render_game(void)
{
    /* Square & "single_color" uniform */
    uniforms[0]->activate(uniforms[0], 0);
    render_mesh(meshes[1], GL_TRIANGLES);

    /* Square & "single_color" uniform */
    uniforms[0]->activate(uniforms[0], 1);
    render_mesh(meshes[1], GL_LINE_LOOP);
    return;
}

