#include "cimmerian.h"

static t_ivec2	get_monitor_size(t_man *man);

int	create_window(t_man *man, const char *title, int width, int height)
{
	man->mlx = mlx_init();
	if (!man->mlx)
		return (0);
	set_resolution(man, get_monitor_size(man), width, height);
	man->window = mlx_new_window(man->mlx, man->res.window_size.x,
			man->res.window_size.y, (char *)title);
	set_viewport(man, man->res.window_size);
	if (!man->window)
	{
		mlx_destroy_display(man->mlx);
		free(man->mlx);
	}
	return (1);
}

static t_ivec2	get_monitor_size(t_man *man)
{
	t_ivec2	monitor_size;

	mlx_get_screen_size(man->mlx, &monitor_size.x, &monitor_size.y);
	return (monitor_size);
}
