#include "cimmerian.h"

void	toggle_fullscreen(t_man *man)
{
	(void)man;
	/*
	t_ivec2	size;
	int		thickness;

	mlx_get_screen_size(man->mlx, &size.x, &size.y);
	thickness = size.x / RES_WIDTH;
	if (thickness > 1)
	{
		--thickness;
		set_ivec2(&size, RES_WIDTH * thickness, RES_HEIGHT * thickness);
	}
	if (man->size.x == size.x)
		set_ivec2(&size, RES_WIDTH, RES_HEIGHT);
	mlx_destroy_window(man->mlx, man->window);
	mlx_destroy_image(man->mlx, man->frame[0].img);
	mlx_destroy_image(man->mlx, man->frame[1].img);
	mlx_destroy_image(man->mlx, man->frame[2].img);
	if (!create_window(man, size.x, size.y))
	{
		deinit(man);
		exit(1);
	}
	*/
	return ;
}
