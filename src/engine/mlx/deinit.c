#include "cimmerian.h"

void	deinit(t_man *man)
{
	if (!man)
		return ;
	if (man->window)
		mlx_destroy_window(man->mlx, man->window);
	free_frames(man);
	mlx_destroy_display(man->mlx);
	free(man->mlx);
	free_images(man);
	free_maps(man);
	return ;
}
