#include "../hdr/gl_functions.h"

GL_ATTACHSHADER glAttachShader;
GL_BINDBUFFER glBindBuffer;
GL_BINDVERTEXARRAY glBindVertexArray;
GL_BUFFERDATA glBufferData;
GL_COMPILESHADER glCompileShader;
GL_CREATEPROGRAM glCreateProgram;
GL_CREATESHADER glCreateShader;
GL_DELETEBUFFERS glDeleteBuffers;
GL_DELETEPROGRAM glDeleteProgram;
GL_DELETESHADER glDeleteShader;
GL_DELETEVERTEXARRAYS glDeleteVertexArrays;
GL_ENABLEVERTEXARRAYATTRIB glEnableVertexArrayAttrib;
GL_GENBUFFERS glGenBuffers;
GL_GENVERTEXARRAYS glGenVertexArrays;
GL_GETUNIFORMLOCATION glGetUniformLocation;
GL_GETVERTEXATTRIBIUIV glGetVertexAttribIuiv;
GL_LINKPROGRAM glLinkProgram;
GL_SHADERSOURCE glShaderSource;
GL_UNIFORM1I glUniform1i;
GL_USEPROGRAM glUseProgram;
GL_VERTEXATTRIBPOINTER glVertexAttribPointer;

int init_gl_functions(void)
{
	glAttachShader = (GL_ATTACHSHADER)glfwGetProcAddress("glAttachShader");
	glBindBuffer = (GL_BINDBUFFER)glfwGetProcAddress("glBindBuffer");
	glBindVertexArray = (GL_BINDVERTEXARRAY)glfwGetProcAddress("glBindVertexArray");
	glBufferData = (GL_BUFFERDATA)glfwGetProcAddress("glBufferData");
	glCompileShader = (GL_COMPILESHADER)glfwGetProcAddress("glCompileShader");
	glCreateProgram = (GL_CREATEPROGRAM)glfwGetProcAddress("glCreateProgram");
	glCreateShader = (GL_CREATESHADER)glfwGetProcAddress("glCreateShader");
	glDeleteBuffers = (GL_DELETEBUFFERS)glfwGetProcAddress("glDeleteBuffers");
	glDeleteProgram = (GL_DELETEPROGRAM)glfwGetProcAddress("glDeleteProgram");
	glDeleteShader = (GL_DELETESHADER)glfwGetProcAddress("glDeleteShader");
	glDeleteVertexArrays = (GL_DELETEVERTEXARRAYS)glfwGetProcAddress("glDeleteVertexArrays");
	glEnableVertexArrayAttrib = (GL_ENABLEVERTEXARRAYATTRIB)glfwGetProcAddress("glEnableVertexArrayAttrib");
	glGenBuffers = (GL_GENBUFFERS)glfwGetProcAddress("glGenBuffers");
	glGenVertexArrays = (GL_GENVERTEXARRAYS)glfwGetProcAddress("glGenVertexArrays");
	glGetUniformLocation = (GL_GETUNIFORMLOCATION)glfwGetProcAddress("glGetUniformLocation");
	glGetVertexAttribIuiv = (GL_GETVERTEXATTRIBIUIV)glfwGetProcAddress("glGetVertexAttribIuiv");
	glLinkProgram = (GL_LINKPROGRAM)glfwGetProcAddress("glLinkProgram");
	glShaderSource = (GL_SHADERSOURCE)glfwGetProcAddress("glShaderSource");
	glUniform1i = (GL_UNIFORM1I)glfwGetProcAddress("glUniform1i");
	glUseProgram = (GL_USEPROGRAM)glfwGetProcAddress("glUseProgram");
	glVertexAttribPointer = (GL_VERTEXATTRIBPOINTER)glfwGetProcAddress("glVertexAttribPointer");

	return glAttachShader && glBindBuffer && glBindVertexArray && glBufferData 
		&& glCompileShader && glCreateProgram && glCreateShader 
		&& glDeleteBuffers && glDeleteProgram && glDeleteShader 
		&& glDeleteVertexArrays && glEnableVertexArrayAttrib && glGenBuffers 
		&& glGenVertexArrays && glGetUniformLocation && glGetVertexAttribIuiv 
		&& glLinkProgram && glShaderSource && glUniform1i && glUseProgram 
		&& glVertexAttribPointer;
}

