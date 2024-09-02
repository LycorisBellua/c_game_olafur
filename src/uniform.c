#include "cimmerian.h"

static GLint loc = 0;

int create_uniform(void)
{
    char* name;

    name = "frame_texture";
    loc = glGetUniformLocation(man.shader_program, name);
    if (loc < 0)
    {
        fprintf(stderr, "Error: The \"%s\" uniform is either not found or "
            "unused in shader program ID°%d\n", name, man.shader_program);
        return 0;
    }
    glUniform1i(loc, 0);
    return 1;
}

void free_uniform(void)
{
    loc = 0;
    return;
}
