#include "cimmerian.h"

void	deinit(t_man *man)
{
	if (man->window)
		mlx_destroy_window(man->mlx, man->window);
	if (man->frame[0].img)
		mlx_destroy_image(man->mlx, man->frame[0].img);
	if (man->frame[1].img)
		mlx_destroy_image(man->mlx, man->frame[1].img);
	mlx_destroy_display(man->mlx);
	free(man->mlx);
	free_images(man);
	free_map(man);
	return ;
}
