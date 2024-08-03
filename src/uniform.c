#include "../hdr/uniform.h"
#include "../hdr/shader_program.h"
#include "../hdr/gl_functions.h"

static GLint loc = 0;

void create_uniform(void)
{
    const char* name = "frame_texture";

    loc = glGetUniformLocation(shader_program, name);
    if (loc < 0)
    {
        fprintf(stderr, "Error: The \"%s\" uniform is either not found or "
            "unused in shader program ID°%d\n", name, shader_program);
        exit(EXIT_FAILURE);
    }

    glUniform1i(loc, 0);
    return;
}

void free_uniform(void)
{
    loc = 0;
    return;
}

