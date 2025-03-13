#include "cimmerian.h"

int	create_uniform(t_man *man)
{
	const char	*name = "frame_texture";

	man->uniform_loc = glGetUniformLocation(man->shader_program, name);
	if (man->uniform_loc < 0)
	{
		dprintf(STDERR_FILENO, "Error: The \"%s\" uniform is either not found "
			"or unused in shader program ID°%d\n", name, man->shader_program);
		return (0);
	}
	glUniform1i(man->uniform_loc, 0);
	return (1);
}

void	free_uniform(t_man *man)
{
	man->uniform_loc = 0;
	return ;
}
