#include "cimmerian.h"

static GLuint	compile_shader(GLenum type, const char *filepath);
static int		get_app_glsl_version(void);
static void		set_glsl_version_in_shader(char *ptr_shader, int version);

/*
	At the end the shaders are freed. This is because they are already compiled 
	in the shader programs, so no need to keep them around unless you want to 
	use them in another shader program.
*/
int	create_shader_program(void)
{
	GLuint	shaders[2];
	int		shader_program;

	shaders[0] = compile_shader(GL_VERTEX_SHADER, PATH_VERTEX_SHADER);
	shaders[1] = compile_shader(GL_FRAGMENT_SHADER, PATH_FRAGMENT_SHADER);
	if (!shaders[0] || !shaders[1])
	{
		glDeleteShader(shaders[0]);
		glDeleteShader(shaders[1]);
		return (put_error(0, E_FAIL_SHADER, 0));
	}
	shader_program = glCreateProgram();
	if (!shader_program)
	{
		glDeleteShader(shaders[0]);
		glDeleteShader(shaders[1]);
		return (put_error(0, E_FAIL_SHADER_PROG, 0));
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

static GLuint	compile_shader(GLenum type, const char *filepath)
{
	static int	app_glsl_version;
	GLuint		id_shader;
	char		*ptr;

	ptr = read_file(filepath);
	if (!ptr)
		return (0);
	else if (type != GL_VERTEX_SHADER && type != GL_FRAGMENT_SHADER)
	{
		free(ptr);
		return (0);
	}
	if (!app_glsl_version)
		app_glsl_version = get_app_glsl_version();
	set_glsl_version_in_shader(ptr, app_glsl_version);
	id_shader = glCreateShader(type);
	glShaderSource(id_shader, 1, (const GLchar **)&ptr, 0);
	glCompileShader(id_shader);
	free(ptr);
	return (id_shader);
}

/*
	GLSL (OpenGL Shader Language)
	----------------------------------------------------------------------------
	There is a deprecated way to render graphics in OpenGL, but the modern way 
	is to use shaders. There are two common sorts: vertex (shape) and fragment 
	(color) shaders. Shaders are written in a language called GLSL, and are 
	stored in a *.glsl text file, although it's possible to simply hardcode 
	them in the source code, as they'll end up in a string anyway once the file 
	is processed.

	No matter the type of shader, its first line is the GLSL version, like so 
	"#version 400\n". The GLSL version must be compatible with OpenGL's 
	version. Here, it doesn't mean which version the dev wishes to use. We can 
	encourage the OS to go with a specific version for this app, but ultimately 
	it's not our decision, so we just retrieve the information and adapt the 
	GLSL's version to match.

	- OpenGL 4 and above   --> GLSL 400
	- OpenGL 3.3           --> GLSL 330
	- OpenGL 3.2 and below --> GLSL 150

	`glGetString(GL_VERSION)` returns "4.6.0 [...]". 
*/
static int	get_app_glsl_version(void)
{
	const unsigned char	*gl = glGetString(GL_VERSION);

	if (gl[0] - 48 >= 4)
		return (400);
	else if (gl[0] - 48 == 3 && gl[2] - 48 == 3)
		return (330);
	return (150);
}

/* The first line of a shader is something like: "#version 400 core\n" */
static void	set_glsl_version_in_shader(char *ptr_shader, int version)
{
	int	i;
	int	glsl;

	i = 0;
	glsl = version;
	while (ptr_shader[i])
	{
		if (isdigit(ptr_shader[i]))
		{
			ptr_shader[i + 2] = glsl % 10 + '0';
			glsl /= 10;
			ptr_shader[i + 1] = glsl % 10 + '0';
			glsl /= 10;
			ptr_shader[i] = glsl % 10 + '0';
			glsl /= 10;
			break ;
		}
		else if (ptr_shader[i] == '\n')
			break ;
		++i;
	}
	return ;
}
