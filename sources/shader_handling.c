#include "../headers/shader_handling.h"
#include "../headers/file_handling.h"

ShaderProgram* shader_program_default = 0;
static int app_glsl_version = 0;

static ShaderProgram* create_shader_program(GLuint id_vs, GLuint id_fs);
static GLuint compile_shader(const GLenum type, const char* filepath);
static int get_app_glsl_version(void);
static void set_glsl_version_in_shader(char* ptr_shader);
static void free_shader_program(ShaderProgram** shader_program);
static void free_shader(GLuint* id);

int create_shader_programs(void)
{
    const char* vs_filepath = "shaders/vs.glsl";
    const char* fs_filepath = "shaders/fs.glsl";

    GLuint vs = compile_shader(GL_VERTEX_SHADER, vs_filepath);
    GLuint fs = compile_shader(GL_FRAGMENT_SHADER, fs_filepath);

    shader_program_default = create_shader_program(vs, fs);

    /*
        The shaders are already compiled in the shader programs, so no need to 
        keep them around unless you want to use them in another shader program.
    */
    free_shader(&vs);
    free_shader(&fs);

    return shader_program_default != 0;
}

void use_shader_program(const ShaderProgram* instance)
{
    if (instance)
        glUseProgram(instance->id);
    else
        glUseProgram(0);
    return;
}

void free_shader_programs(void)
{
    free_shader_program(&shader_program_default);
    return;
}

static ShaderProgram* create_shader_program(GLuint id_vs, GLuint id_fs)
{
    ShaderProgram* shader_program = 0;

    if (!id_vs || !id_fs)
        return 0;

    shader_program = malloc(sizeof(ShaderProgram));
    if (!shader_program)
        return 0;

    shader_program->id = glCreateProgram();
    if (!shader_program->id)
    {
        fprintf(stderr, "ERROR: Couldn't compile shader program.\n");
        free(shader_program);
        return 0;
    }

    glAttachShader(shader_program->id, id_vs);
    glAttachShader(shader_program->id, id_fs);
    glLinkProgram(shader_program->id);

    return shader_program;
}

static GLuint compile_shader(const GLenum type, const char* filepath)
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
        if (CHAR_IS_DIGIT(ptr_shader[i]))
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

static void free_shader_program(ShaderProgram** shader_program)
{
    glDeleteProgram((*shader_program)->id);
    free(*shader_program);

    /* Nullify the reference to prevent a double free */
    *shader_program = 0;
    return;
}

static void free_shader(GLuint* id)
{
    glDeleteShader(*id);

    /* Nullify the reference to prevent a double free */
    *id = 0;
    return;
}

