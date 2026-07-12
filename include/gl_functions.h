#ifndef GL_FUNCTIONS_H
# define GL_FUNCTIONS_H

# include <GL/gl.h>
# include <GL/glext.h>
# include "lib/glfw3.h"

int	init_gl_functions(void);

typedef void	(APIENTRY *GL_AS)(GLuint program, GLuint shader);
typedef void	(APIENTRY *GL_BB)(GLenum target, GLuint buffer);
typedef void	(APIENTRY *GL_BVA)(GLuint array);
typedef void	(APIENTRY *GL_BD)(GLenum target, GLsizeiptr size,
					const void* data, GLenum usage);
typedef void	(APIENTRY *GL_COS)(GLuint shader);
typedef GLuint	(APIENTRY *GL_CP)(void);
typedef GLuint	(APIENTRY *GL_CS)(GLenum shaderType);
typedef void	(APIENTRY *GL_DB)(GLsizei n, const GLuint* buffers);
typedef void	(APIENTRY *GL_DP)(GLuint program);
typedef void	(APIENTRY *GL_DS)(GLuint shader);
typedef void	(APIENTRY *GL_DVA)(GLsizei n, const GLuint* arrays);
typedef void	(APIENTRY *GL_EVAA)(GLuint vaobj, GLuint index);
typedef void	(APIENTRY *GL_GB)(GLsizei n, GLuint* buffers);
typedef void	(APIENTRY *GL_GVA)(GLsizei n, GLuint* arrays);
typedef GLint	(APIENTRY *GL_GUL)(GLuint program, const GLchar* name);
typedef void	(APIENTRY *GL_GVAI)(GLuint index, GLenum pname, GLuint* params);
typedef void	(APIENTRY *GL_LP)(GLuint program);
typedef void*  (APIENTRY *GL_MB)(GLenum target, GLenum access);
typedef void	(APIENTRY *GL_SS)(GLuint shader, GLsizei count,
					const GLchar** string, const GLint* length);
typedef void	(APIENTRY *GL_U1I)(GLint location, GLint v0);
typedef GLboolean (APIENTRY *GL_UMB)(GLenum target);
typedef void	(APIENTRY *GL_UP)(GLuint program);
typedef void	(APIENTRY *GL_VAP)(GLuint index, GLint size, GLenum type,
					GLboolean normalized, GLsizei stride, const void* pointer);

/*
	These GL 2.0+/3.0+ entry points are loaded at runtime as function pointers.
	libGL exports functions of the same names, so we alias the GL names onto
	prefixed variables (the GLEW/glad approach). This prevents our pointer
	objects from colliding with libGL's exported functions: a clash that is
	harmless under separate compilation but flagged, and best avoided, under LTO.
	glfwGetProcAddress lookups use string literals, so they are unaffected.
*/
# define glAttachShader ol_glAttachShader
# define glBindBuffer ol_glBindBuffer
# define glBindVertexArray ol_glBindVertexArray
# define glBufferData ol_glBufferData
# define glCompileShader ol_glCompileShader
# define glCreateProgram ol_glCreateProgram
# define glCreateShader ol_glCreateShader
# define glDeleteBuffers ol_glDeleteBuffers
# define glDeleteProgram ol_glDeleteProgram
# define glDeleteShader ol_glDeleteShader
# define glDeleteVertexArrays ol_glDeleteVertexArrays
# define glEnableVertexArrayAttrib ol_glEnableVertexArrayAttrib
# define glGenBuffers ol_glGenBuffers
# define glGenVertexArrays ol_glGenVertexArrays
# define glGetUniformLocation ol_glGetUniformLocation
# define glGetVertexAttribIuiv ol_glGetVertexAttribIuiv
# define glLinkProgram ol_glLinkProgram
# define glMapBuffer ol_glMapBuffer
# define glShaderSource ol_glShaderSource
# define glUniform1i ol_glUniform1i
# define glUnmapBuffer ol_glUnmapBuffer
# define glUseProgram ol_glUseProgram
# define glVertexAttribPointer ol_glVertexAttribPointer

extern GL_AS	glAttachShader;
extern GL_BB	glBindBuffer;
extern GL_BVA	glBindVertexArray;
extern GL_BD	glBufferData;
extern GL_COS	glCompileShader;
extern GL_CP	glCreateProgram;
extern GL_CS	glCreateShader;
extern GL_DB	glDeleteBuffers;
extern GL_DP	glDeleteProgram;
extern GL_DS	glDeleteShader;
extern GL_DVA	glDeleteVertexArrays;
extern GL_EVAA	glEnableVertexArrayAttrib;
extern GL_GB	glGenBuffers;
extern GL_GVA	glGenVertexArrays;
extern GL_GUL	glGetUniformLocation;
extern GL_GVAI	glGetVertexAttribIuiv;
extern GL_LP	glLinkProgram;
extern GL_MB   glMapBuffer;
extern GL_SS	glShaderSource;
extern GL_U1I	glUniform1i;
extern GL_UMB  glUnmapBuffer;
extern GL_UP	glUseProgram;
extern GL_VAP	glVertexAttribPointer;

#endif
