#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../headers/shader_handling.h"
#include "../headers/file_handling.h"

static int app_glsl_version = 0;

static int get_app_glsl_version(void);
static void set_glsl_version_in_shader(char* ptr_shader);

/* Call before compiling the first shader */
void set_app_glsl_version(void)
{
    app_glsl_version = get_app_glsl_version();
    return;
}

GLuint compile_shader(const GLenum type, const char* filepath)
{
    GLuint shader;
    char* ptr = read_file(filepath);

    if (!ptr)
        return 0;
    else if (type != GL_VERTEX_SHADER && type != GL_FRAGMENT_SHADER)
    {
        free(ptr);
        return 0;
    }

    set_glsl_version_in_shader(ptr);

    shader = glCreateShader(type);
    glShaderSource(shader, 1, (char const* const*)&ptr, NULL);
    glCompileShader(shader);
    free(ptr);
    return shader;
}

GLuint create_shader_program(GLFWwindow* window, GLuint vs, GLuint fs)
{
    GLuint shader_program;

    if (!vs || !fs)
        return 0;

    shader_program = glCreateProgram();

    if (!shader_program)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        fprintf(stderr, "ERROR: Couldn't compile shader program.\n");
        free_shader(&vs);
        free_shader(&fs);
    }
    else
    {
        glAttachShader(shader_program, vs);
        glAttachShader(shader_program, fs);
        glLinkProgram(shader_program);
    }
    return shader_program;
}

void free_shader(GLuint* shader)
{
    glDeleteShader(*shader);

    /* Nullify the reference to avoid a double free */
    *shader = 0;
    return;
}

void free_shader_program(GLuint* shader_program)
{
    glDeleteProgram(*shader_program);

    /* Nullify the reference to avoid a double free */
    *shader_program = 0;
    return;
}

static int get_app_glsl_version(void)
{
    /*
        GLSL (OpenGL Shader Language)
        -----------------------------------------------------------------------
        There is a deprecated way to render graphics in OpenGL, but the modern 
        way is to use shaders. There are two common sorts: vertex (shape) and 
        fragment (color) shaders. Shaders are written in a language called 
        GLSL, and are stored in a *.glsl text file, although it's possible to 
        simply hardcode them in the source code, as they'll end up in a string 
        anyway once the file is processed.

        No matter the type of shader, its first line is the GLSL version, like 
        so "#version 400\n". The GLSL version must be compatible with OpenGL's 
        version. Here, it doesn't mean which version the dev wishes to use. We 
        can encourage the OS to go with a specific version for this app, but 
        ultimately it's not our decision, so we just retrieve the information 
        and adapt the GLSL's version to match.

        - OpenGL 4 and above   --> GLSL 400
        - OpenGL 3.3           --> GLSL 330
        - OpenGL 3.2 and below --> GLSL 150
    */

    const unsigned char* gl = glGetString(GL_VERSION); /* "4.6.0 [...]" */

    if (gl[0]-48 >= 4)
        return 400;
    else if (gl[0]-48 == 3 && gl[2]-48 == 3)
        return 330;
    else
        return 150;
}

static void set_glsl_version_in_shader(char* ptr_shader)
{
    int i;
    int glsl = app_glsl_version;
    char new_digits[3];

    /* The first line of a shader is something like: "#version 400\n" */

    new_digits[2] = glsl % 10 + '0';
    glsl /= 10;
    new_digits[1] = glsl % 10 + '0';
    glsl /= 10;
    new_digits[0] = glsl % 10 + '0';
    glsl /= 10;

    for (i = 0; ptr_shader[i]; ++i)
    {
        if (ptr_shader[i] >= '0' && ptr_shader[i] <= '9')
        {
            ptr_shader[i] = new_digits[0];
            ptr_shader[i + 1] = new_digits[1];
            ptr_shader[i + 2] = new_digits[2];
            break;
        }
    }
    return;
}

