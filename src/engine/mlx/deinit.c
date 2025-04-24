#include "cimmerian.h"

void	free_frames(t_man *man);

void	deinit(t_man *man)
{
	if (!man)
		return ;
	if (man->window)
	{
		mlx_window_destroy(man->mlx, man->window);
		man->window = 0;
	}
	free_frames(man);
	mlx_destroy_display(man->mlx);
	free(man->mlx);
	man->mlx = 0;
	free_images(man);
	free_maps(man);
	return ;
}
