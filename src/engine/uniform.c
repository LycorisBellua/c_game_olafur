#include "cimmerian.h"

int	create_uniform(void)
{
	const char	*name = "frame_texture";

	g_man.uniform_loc = glGetUniformLocation(g_man.shader_program, name);
	if (g_man.uniform_loc < 0)
	{
		dprintf(STDERR_FILENO, "Error: The \"%s\" uniform is either not found "
			"or unused in shader program ID°%d\n", name, g_man.shader_program);
		return (0);
	}
	glUniform1i(g_man.uniform_loc, 0);
	return (1);
}

void	free_uniform(void)
{
	g_man.uniform_loc = 0;
	return ;
}
