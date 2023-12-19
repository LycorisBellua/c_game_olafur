#include <GL/glew.h>
#include "../headers/rendering.h"

void render_mesh(const GLuint shader_program, const GLuint VAO, 
    const GLenum drawing_mode, const int nbr_vertices)
{
    /* Drawing mode example: GL_TRIANGLES */
    int start_index = 0;

    glUseProgram(shader_program);
    glBindVertexArray(VAO);
    glDrawArrays(drawing_mode, start_index, nbr_vertices);
    return;
}

