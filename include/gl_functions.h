#ifndef GL_FUNCTIONS_H
#define GL_FUNCTIONS_H

#include <GL/gl.h>
#include <GLFW/glfw3.h>

int init_gl_functions(void);

typedef void   (*GL_AS)(GLuint program, GLuint shader);
typedef void   (*GL_BB)(GLenum target, GLuint buffer);
typedef void   (*GL_BVA)(GLuint array);
typedef void   (*GL_BD)(GLenum target, GLsizeiptr size, const void* data, 
                        GLenum usage);
typedef void   (*GL_COS)(GLuint shader);
typedef GLuint (*GL_CP)(void);
typedef GLuint (*GL_CS)(GLenum shaderType);
typedef void   (*GL_DB)(GLsizei n, const GLuint* buffers);
typedef void   (*GL_DP)(GLuint program);
typedef void   (*GL_DS)(GLuint shader);
typedef void   (*GL_DVA)(GLsizei n, const GLuint* arrays);
typedef void   (*GL_EVAA)(GLuint vaobj, GLuint index);
typedef void   (*GL_GB)(GLsizei n, GLuint* buffers);
typedef void   (*GL_GVA)(GLsizei n, GLuint* arrays);
typedef GLint  (*GL_GUL)(GLuint program, const GLchar* name);
typedef void   (*GL_GVAI)(GLuint index, GLenum pname, GLuint* params);
typedef void   (*GL_LP)(GLuint program);
typedef void   (*GL_SS)(GLuint shader, GLsizei count, const GLchar** string, 
                        const GLint* length);
typedef void   (*GL_U1I)(GLint location, GLint v0);
typedef void   (*GL_UP)(GLuint program);
typedef void   (*GL_VAP)(GLuint index, GLint size, GLenum type, 
                         GLboolean normalized, GLsizei stride, 
                         const void* pointer);

extern GL_AS   glAttachShader;
extern GL_BB   glBindBuffer;
extern GL_BVA  glBindVertexArray;
extern GL_BD   glBufferData;
extern GL_COS  glCompileShader;
extern GL_CP   glCreateProgram;
extern GL_CS   glCreateShader;
extern GL_DB   glDeleteBuffers;
extern GL_DP   glDeleteProgram;
extern GL_DS   glDeleteShader;
extern GL_DVA  glDeleteVertexArrays;
extern GL_EVAA glEnableVertexArrayAttrib;
extern GL_GB   glGenBuffers;
extern GL_GVA  glGenVertexArrays;
extern GL_GUL  glGetUniformLocation;
extern GL_GVAI glGetVertexAttribIuiv;
extern GL_LP   glLinkProgram;
extern GL_SS   glShaderSource;
extern GL_U1I  glUniform1i;
extern GL_UP   glUseProgram;
extern GL_VAP  glVertexAttribPointer;

#endif
