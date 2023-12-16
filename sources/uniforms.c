#include <GL/glew.h>
#include "../headers/uniforms.h"

UniformStruct init_uniform(const GLuint shader_program, const char* name, 
    const UniformCallback activate, 
    const float x, const float y, const float z, const float w)
{
    UniformStruct u;
    u.loc = glGetUniformLocation(shader_program, name);
    u.activate = activate;
    u.x = x;
    u.y = y;
    u.z = z;
    u.w = w;
    return u;
}

void activate_uniform_3f(const UniformStruct* u, const int activate)
{
    if (activate)
        glUniform3f(u->loc, u->x, u->y, u->z);
    else
        glUniform3f(u->loc, 0, 0, 0);
    return;
}

