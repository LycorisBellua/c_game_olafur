#include "olafur.h"

void	deinit(t_man *man)
{
	if (!man)
		return ;
	if (man->gl_loaded)
	{
		free_shader_program(man);
		free_mesh();
	}
	free_uniform(man);
	free_frame(man);
	free_images(man);
	free_maps(man);
	audio_deinit(&man->audio);
	glfwTerminate();
	man->window = 0;
	man->gl_loaded = 0;
	return ;
}
