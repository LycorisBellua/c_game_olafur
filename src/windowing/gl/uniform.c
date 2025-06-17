#include "olafur.h"

int	create_uniform(t_man *man)
{
	man->uniform_loc = glGetUniformLocation(man->shader_program,
			"frame_texture");
	if (man->uniform_loc < 0)
		return (put_error(0, E_FAIL_UNIFORM_FRAME_TEXTURE, 0, 0));
	glUniform1i(man->uniform_loc, 0);
	return (1);
}

void	free_uniform(t_man *man)
{
	man->uniform_loc = 0;
	return ;
}
