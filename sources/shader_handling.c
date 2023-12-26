#include "../headers/shader_handling.h"
#include "../headers/file_handling.h"

GLuint id_shader_program_world = 0;
GLuint id_shader_program_ui = 0;
static int app_glsl_version = 0;

static int get_app_glsl_version(void);
static void set_glsl_version_in_shader(char* ptr_shader);

GLuint compile_shader(const GLenum type, const char* filepath)
{
    GLuint id_shader;
    char* ptr = read_file(filepath);

    if (!ptr)
        return 0;
    else if (type != GL_VERTEX_SHADER && type != GL_FRAGMENT_SHADER)
    {
        free(ptr);
        return 0;
    }

    if (!app_glsl_version)
        app_glsl_version = get_app_glsl_version();
    set_glsl_version_in_shader(ptr);

    id_shader = glCreateShader(type);
    glShaderSource(id_shader, 1, (const char* const*)&ptr, 0);
    glCompileShader(id_shader);
    free(ptr);
    return id_shader;
}

GLuint create_shader_program(GLuint id_vs, GLuint id_fs)
{
    GLuint id_shader_program;

    if (!id_vs || !id_fs)
        return 0;

    id_shader_program = glCreateProgram();

    if (!id_shader_program)
    {
        fprintf(stderr, "ERROR: Couldn't compile shader program.\n");
        free_shader(&id_vs);
        free_shader(&id_fs);
    }
    else
    {
        glAttachShader(id_shader_program, id_vs);
        glAttachShader(id_shader_program, id_fs);
        glLinkProgram(id_shader_program);
    }
    return id_shader_program;
}

void free_shader(GLuint* id)
{
    glDeleteShader(*id);

    /* Nullify the reference to prevent a double free */
    *id = 0;
    return;
}

void free_shader_program(GLuint* id)
{
    glDeleteProgram(*id);

    /* Nullify the reference to prevent a double free */
    *id = 0;
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

    /* The first line of a shader is something like: "#version 400 core\n" */

    for (i = 0; ptr_shader[i]; ++i)
    {
        if (char_is_digit(ptr_shader[i]))
        {
            ptr_shader[i + 2] = glsl % 10 + '0';
            glsl /= 10;

            ptr_shader[i + 1] = glsl % 10 + '0';
            glsl /= 10;

            ptr_shader[i] = glsl % 10 + '0';
            glsl /= 10;
            break;
        }
        else if (ptr_shader[i] == '\n')
            break;
    }
    return;
}

