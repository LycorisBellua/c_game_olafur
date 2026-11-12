#include "olafur.h"

void	free_shader_program(t_man *man);
void	free_uniform(t_man *man);
void	free_mesh(void);
void	free_frame(t_man *man);

/*
	GL objects can only be freed once the GL functions are loaded, and while
	their context exists, so glfwTerminate() comes last.
*/
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
