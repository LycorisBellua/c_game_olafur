#include "gl_functions.h"

static int init_gl_functions_for_shaders(void);
static int init_gl_functions_for_buffers(void);
static int init_gl_functions_for_programs(void);
static int init_gl_functions_for_vertices(void);

GL_AS   glAttachShader;
GL_BB   glBindBuffer;
GL_BVA  glBindVertexArray;
GL_BD   glBufferData;
GL_COS  glCompileShader;
GL_CP   glCreateProgram;
GL_CS   glCreateShader;
GL_DB   glDeleteBuffers;
GL_DP   glDeleteProgram;
GL_DS   glDeleteShader;
GL_DVA  glDeleteVertexArrays;
GL_EVAA glEnableVertexArrayAttrib;
GL_GB   glGenBuffers;
GL_GVA  glGenVertexArrays;
GL_GUL  glGetUniformLocation;
GL_GVAI glGetVertexAttribIuiv;
GL_LP   glLinkProgram;
GL_SS   glShaderSource;
GL_U1I  glUniform1i;
GL_UP   glUseProgram;
GL_VAP  glVertexAttribPointer;

int init_gl_functions(void)
{
    return init_gl_functions_for_shaders() 
        && init_gl_functions_for_buffers() 
        && init_gl_functions_for_programs() 
        && init_gl_functions_for_vertices();
}

static int init_gl_functions_for_shaders(void)
{
    glAttachShader = (GL_AS)glfwGetProcAddress("glAttachShader");
    glCompileShader = (GL_COS)glfwGetProcAddress("glCompileShader");
    glCreateShader = (GL_CS)glfwGetProcAddress("glCreateShader");
    glDeleteShader = (GL_DS)glfwGetProcAddress("glDeleteShader");
    glShaderSource = (GL_SS)glfwGetProcAddress("glShaderSource");

    return glAttachShader && glCompileShader && glCreateShader && 
        glDeleteShader && glShaderSource;
}

static int init_gl_functions_for_buffers(void)
{
    glBindBuffer = (GL_BB)glfwGetProcAddress("glBindBuffer");
    glBufferData = (GL_BD)glfwGetProcAddress("glBufferData");
    glDeleteBuffers = (GL_DB)glfwGetProcAddress("glDeleteBuffers");
    glGenBuffers = (GL_GB)glfwGetProcAddress("glGenBuffers");

    return glBindBuffer && glBufferData && glDeleteBuffers && glGenBuffers;
}

static int init_gl_functions_for_programs(void)
{
    glCreateProgram = (GL_CP)glfwGetProcAddress("glCreateProgram");
    glDeleteProgram = (GL_DP)glfwGetProcAddress("glDeleteProgram");
    glLinkProgram = (GL_LP)glfwGetProcAddress("glLinkProgram");
    glUseProgram = (GL_UP)glfwGetProcAddress("glUseProgram");

    return glCreateProgram && glDeleteProgram && glLinkProgram && glUseProgram;
}

static int init_gl_functions_for_vertices(void)
{
    glBindVertexArray = (GL_BVA)glfwGetProcAddress("glBindVertexArray");
    glDeleteVertexArrays = (GL_DVA)glfwGetProcAddress("glDeleteVertexArrays");
    glEnableVertexArrayAttrib = 
        (GL_EVAA)glfwGetProcAddress("glEnableVertexArrayAttrib");
    glGenVertexArrays = (GL_GVA)glfwGetProcAddress("glGenVertexArrays");

    glGetVertexAttribIuiv = 
        (GL_GVAI)glfwGetProcAddress("glGetVertexAttribIuiv");
    glVertexAttribPointer = (GL_VAP)glfwGetProcAddress("glVertexAttribPointer");

    glGetUniformLocation = (GL_GUL)glfwGetProcAddress("glGetUniformLocation");
    glUniform1i = (GL_U1I)glfwGetProcAddress("glUniform1i");

    return glBindVertexArray && glDeleteVertexArrays 
        && glEnableVertexArrayAttrib && glGenVertexArrays 
        && glGetVertexAttribIuiv && glVertexAttribPointer 
        && glGetUniformLocation && glUniform1i;
}
