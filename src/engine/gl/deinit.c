#include "cimmerian.h"

void	deinit(t_man *man)
{
	glfwTerminate();
	free_shader_program(man);
	free_uniform(man);
	free_mesh();
	free_frames(man);
	free_images(man);
	free_map(man);
	return ;
}
