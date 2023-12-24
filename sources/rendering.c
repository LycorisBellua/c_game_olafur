#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../headers/meshes.h"
#include "../headers/rendering.h"
#include "../headers/uniforms.h"
#include "../headers/input.h"
#include "../headers/windowing.h"

float player_speed[3] = { -0.01f, -0.01f, 0.007f };

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
    /* Background representing the viewport:
    uniforms[1]->activate(uniforms[1], 0);
    render_mesh(meshes[3], GL_TRIANGLES);
    uniforms[1]->activate(uniforms[1], 1);
    */

    /* Triangle & "single_color" uniform */
    uniforms[0]->activate(uniforms[0], 0);
    render_mesh(meshes[2], GL_TRIANGLES);
    return;
}

void render_game(void)
{
    /* Square & "single_color" uniform */
    uniforms[0]->activate(uniforms[0], 0);
    render_mesh(meshes[4], GL_TRIANGLES);

    /* Square & "single_color" uniform */
    uniforms[0]->activate(uniforms[0], 1);
    render_mesh(meshes[4], GL_LINE_LOOP);
    return;
}

void move_player(void)
{
    float yaw;

    /* "pos_offset" uniform */
    *((float*)uniforms[1]->data + 0) += movement_action[0] * player_speed[0];
    *((float*)uniforms[1]->data + 1) += movement_action[1] * player_speed[1];

    /* "yaw" uniform (rotation around Y axis) */
    yaw = *((float*)uniforms[2]->data + 0) + rotation_action * player_speed[1];
    if (yaw < -360.0f)
        yaw += 360.0f;
    else if (yaw > 360.0f)
        yaw -= 360.0f;
    *((float*)uniforms[2]->data + 0) = yaw;
    uniforms[2]->activate(uniforms[2], 1);

    /* "scale_factor" uniform */
    *((float*)uniforms[3]->data + 0) += movement_action[2] * player_speed[2];
    uniforms[3]->activate(uniforms[3], 1);
    return;
}

