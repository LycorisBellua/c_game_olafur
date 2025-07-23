#include "olafur.h"

static GLuint	compile_shader(GLenum type, const char *text);

const char	*g_vertex_shader =
"#version 400 core\n"
"layout(location = 0) in vec2 in_pos;\n"
"layout(location = 1) in vec2 in_tex_coord;\n"
"out vec2 frag_tex_coord;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(in_pos, 0.0, 1.0);\n"
"	frag_tex_coord = in_tex_coord;\n"
"}\n";

const char	*g_fragment_shader =
"#version 400 core\n"
"in vec2 frag_tex_coord;\n"
"uniform sampler2D frame_texture;\n"
"out vec4 frag_color;\n"
"void main()\n"
"{\n"
"	vec3 colors = texture(frame_texture, frag_tex_coord).rgb;\n"
"	frag_color = vec4(colors, 1.0);\n"
"}\n";

/*
	At the end the shaders are freed. This is because they are already compiled 
	in the shader programs, so no need to keep them around unless you want to 
	use them in another shader program.
*/
int	create_shader_program(void)
{
	GLuint	shaders[2];
	int		shader_program;

	shaders[0] = compile_shader(GL_VERTEX_SHADER, g_vertex_shader);
	shaders[1] = compile_shader(GL_FRAGMENT_SHADER, g_fragment_shader);
	if (!shaders[0] || !shaders[1])
	{
		glDeleteShader(shaders[0]);
		glDeleteShader(shaders[1]);
		return (put_error(0, E_FAIL_SHADER, 0, 0));
	}
	shader_program = glCreateProgram();
	if (!shader_program)
	{
		glDeleteShader(shaders[0]);
		glDeleteShader(shaders[1]);
		return (put_error(0, E_FAIL_SHADER_PROG, 0, 0));
	}
	glAttachShader(shader_program, shaders[0]);
	glAttachShader(shader_program, shaders[1]);
	glLinkProgram(shader_program);
	glUseProgram(shader_program);
	glDeleteShader(shaders[0]);
	glDeleteShader(shaders[1]);
	return (shader_program);
}

void	free_shader_program(t_man *man)
{
	glUseProgram(0);
	glDeleteProgram(man->shader_program);
	man->shader_program = 0;
	return ;
}

static GLuint	compile_shader(GLenum type, const char *text)
{
	GLuint	id_shader;

	if ((type != GL_VERTEX_SHADER && type != GL_FRAGMENT_SHADER) || !text)
		return (0);
	id_shader = glCreateShader(type);
	glShaderSource(id_shader, 1, (const GLchar **)&text, 0);
	glCompileShader(id_shader);
	return (id_shader);
}
