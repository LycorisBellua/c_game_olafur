#include "cimmerian.h"

void	free_frames(t_man *man);

void	toggle_fullscreen(t_man *man)
{
	mlx_destroy_window(man->mlx, man->window);
	free_frames(man);
	if (man->res.window_size.x == man->res.monitor_size.x)
	{
		man->res.window_size.x = man->res.window_size_default.x;
		man->res.window_size.y = man->res.window_size_default.y;
	}
	else
	{
		man->res.window_size.x = man->res.monitor_size.x;
		man->res.window_size.y = man->res.monitor_size.y;
	}
	man->window = mlx_new_window(man->mlx, man->res.window_size.x,
			man->res.window_size.y, man->title);
	set_viewport(man, man->res.window_size);
	if (!man->window || !init_frames(man))
		exit(put_error(man, 0, EXIT_FAILURE));
	init_input_handling(man);
	return ;
}

t_ivec2	get_monitor_size(t_man *man)
{
	t_ivec2	size;

	mlx_get_screen_size(man->mlx, &size.x, &size.y);
	return (size);
}
