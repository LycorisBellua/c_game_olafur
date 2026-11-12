#include "olafur.h"

static GLuint	compile_shader(GLenum type, const char *text);
static GLuint	link_program(GLuint vertex_shader, GLuint fragment_shader);
static int		check_status(GLuint id, int is_program);

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
	The shaders are freed at the end, whether linking succeeded or not. Once
	linked, they live in the shader program, so they're no longer needed
	unless you want to use them in another shader program.
*/
int	create_shader_program(void)
{
	GLuint	shaders[2];
	GLuint	shader_program;

	shaders[0] = compile_shader(GL_VERTEX_SHADER, g_vertex_shader);
	shaders[1] = compile_shader(GL_FRAGMENT_SHADER, g_fragment_shader);
	shader_program = 0;
	if (shaders[0] && shaders[1])
		shader_program = link_program(shaders[0], shaders[1]);
	glDeleteShader(shaders[0]);
	glDeleteShader(shaders[1]);
	if (shader_program)
		glUseProgram(shader_program);
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
	if (!id_shader)
		return (put_error(E_FAIL_SHADER, 0, 0));
	glShaderSource(id_shader, 1, (const GLchar **)&text, 0);
	glCompileShader(id_shader);
	if (!check_status(id_shader, 0))
	{
		glDeleteShader(id_shader);
		return (0);
	}
	return (id_shader);
}

static GLuint	link_program(GLuint vertex_shader, GLuint fragment_shader)
{
	GLuint	program;

	program = glCreateProgram();
	if (!program)
		return (put_error(E_FAIL_SHADER_PROG, 0, 0));
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);
	if (!check_status(program, 1))
	{
		glDeleteProgram(program);
		return (0);
	}
	return (program);
}

/* On failure, the driver's log is printed, since it says what went wrong. */
static int	check_status(GLuint id, int is_program)
{
	GLint	status;
	char	log[1024];

	if (is_program)
		glGetProgramiv(id, GL_LINK_STATUS, &status);
	else
		glGetShaderiv(id, GL_COMPILE_STATUS, &status);
	if (status)
		return (1);
	log[0] = 0;
	if (is_program)
		glGetProgramInfoLog(id, sizeof(log), 0, log);
	else
		glGetShaderInfoLog(id, sizeof(log), 0, log);
	if (is_program)
		return (put_error(E_FAIL_SHADER_PROG, log, 0));
	return (put_error(E_FAIL_SHADER, log, 0));
}
