#include "cimmerian.h"

static void	decide_on_new_window_size(t_man *man);

void	action_toggle_fullscreen(t_man *man)
{
	decide_on_new_window_size(man);
	mlx_destroy_window(man->mlx, man->window);
	free_frames(man);
	man->window = mlx_new_window(man->mlx, man->res.window_size.x,
			man->res.window_size.y, man->title);
	set_viewport(man, man->res.window_size);
	if (!man->window)
	{
		deinit(man);
		exit(1);
	}
	init_frames(man);
	init_input_handling(man);
	return ;
}

void	action_toggle_debug(t_man *man)
{
	man->show_debug = !man->show_debug;
	return ;
}

void	action_close_window(t_man *man)
{
	cross_window_button_callback(man);
	return ;
}

static void	decide_on_new_window_size(t_man *man)
{
	t_ivec2	size;
	int		thickness;

	mlx_get_screen_size(man->mlx, &size.x, &size.y);
	thickness = size.x / man->res.window_size_default.x;
	if (thickness > 1)
	{
		--thickness;
		size.x = man->res.window_size_default.x * thickness;
		size.y = man->res.window_size_default.y * thickness;
	}
	if (man->res.window_size.x == size.x)
	{
		size.x = man->res.window_size_default.x;
		size.y = man->res.window_size_default.y;
	}
	set_ivec2(&man->res.window_size, size.x, size.y);
	return ;
}
