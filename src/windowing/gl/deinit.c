#include "cimmerian.h"

void	free_shader_program(t_man *man);
void	free_uniform(t_man *man);
void	free_mesh(void);
void	free_frame(t_man *man);

void	deinit(t_man *man)
{
	if (!man)
		return ;
	glfwTerminate();
	free_shader_program(man);
	free_uniform(man);
	free_mesh();
	free_frame(man);
	free_images(man);
	free_maps(man);
	audio_deinit(&man->audio);
	return ;
}
