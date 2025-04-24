#include "cimmerian.h"

void	free_frames(t_man *man);

void	toggle_fullscreen(t_man *man)
{
	static int		fullscreen;
	static t_ivec2	prev_size;
	static t_ivec2	prev_pos;
	t_ivec2			new_size;
	t_ivec2			new_pos;

	fullscreen = !fullscreen;
	if (fullscreen)
	{
		prev_size = man->res.window_size;
		prev_pos = man->res.window_position;
		new_size = man->res.monitor_size;
		set_ivec2(&new_pos, 0, 0);
	}
	else
	{
		new_size = prev_size;
		new_pos = prev_pos;
	}
	mlx_resize_window(man->mlx, man->window, new_size.x, new_size.y);
	mlx_move_window(man->mlx, man->window, new_pos.x, new_pos.y);
	return ;
}

t_ivec2	get_monitor_size(t_man *man)
{
	t_ivec2	size;

	mlx_get_screen_size(man->mlx, &size.x, &size.y);
	return (size);
}
