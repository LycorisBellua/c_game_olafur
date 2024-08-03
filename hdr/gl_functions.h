#ifndef __GL_FUNCTIONS_H__
#define __GL_FUNCTIONS_H__

#ifndef __GL_H__
#define __GL_H__
#include <GL/gl.h>
#endif

#ifndef __GLFW_H__
#define __GLFW_H__
#include <GLFW/glfw3.h>
#endif

typedef void (*GL_ATTACHSHADER)(GLuint program, GLuint shader);
typedef void (*GL_BINDBUFFER)(GLenum target, GLuint buffer);
typedef void (*GL_BINDVERTEXARRAY)(GLuint array);
typedef void (*GL_BUFFERDATA)(GLenum target, GLsizeiptr size, const void* data, GLenum usage);
typedef void (*GL_COMPILESHADER)(GLuint shader);
typedef GLuint (*GL_CREATEPROGRAM)(void);
typedef GLuint (*GL_CREATESHADER)(GLenum shaderType);
typedef void (*GL_DELETEBUFFERS)(GLsizei n, const GLuint* buffers);
typedef void (*GL_DELETEPROGRAM)(GLuint program);
typedef void (*GL_DELETESHADER)(GLuint shader);
typedef void (*GL_DELETEVERTEXARRAYS)(GLsizei n, const GLuint* arrays);
typedef void (*GL_ENABLEVERTEXARRAYATTRIB)(GLuint vaobj, GLuint index);
typedef void (*GL_GENBUFFERS)(GLsizei n, GLuint* buffers);
typedef void (*GL_GENVERTEXARRAYS)(GLsizei n, GLuint* arrays);
typedef GLint (*GL_GETUNIFORMLOCATION)(GLuint program, const GLchar* name);
typedef void (*GL_GETVERTEXATTRIBIUIV)(GLuint index, GLenum pname, GLuint* params);
typedef void (*GL_LINKPROGRAM)(GLuint program);
typedef void (*GL_SHADERSOURCE)(GLuint shader, GLsizei count, const GLchar** string, const GLint* length);
typedef void (*GL_UNIFORM1I)(GLint location, GLint v0);
typedef void (*GL_USEPROGRAM)(GLuint program);
typedef void (*GL_VERTEXATTRIBPOINTER)(	GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);

extern GL_ATTACHSHADER glAttachShader;
extern GL_BINDBUFFER glBindBuffer;
extern GL_BINDVERTEXARRAY glBindVertexArray;
extern GL_BUFFERDATA glBufferData;
extern GL_COMPILESHADER glCompileShader;
extern GL_CREATEPROGRAM glCreateProgram;
extern GL_CREATESHADER glCreateShader;
extern GL_DELETEBUFFERS glDeleteBuffers;
extern GL_DELETEPROGRAM glDeleteProgram;
extern GL_DELETESHADER glDeleteShader;
extern GL_DELETEVERTEXARRAYS glDeleteVertexArrays;
extern GL_ENABLEVERTEXARRAYATTRIB glEnableVertexArrayAttrib;
extern GL_GENBUFFERS glGenBuffers;
extern GL_GENVERTEXARRAYS glGenVertexArrays;
extern GL_GETUNIFORMLOCATION glGetUniformLocation;
extern GL_GETVERTEXATTRIBIUIV glGetVertexAttribIuiv;
extern GL_LINKPROGRAM glLinkProgram;
extern GL_SHADERSOURCE glShaderSource;
extern GL_UNIFORM1I glUniform1i;
extern GL_USEPROGRAM glUseProgram;
extern GL_VERTEXATTRIBPOINTER glVertexAttribPointer;

int init_gl_functions(void);

#endif
