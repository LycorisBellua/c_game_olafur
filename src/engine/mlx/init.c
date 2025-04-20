#include "cimmerian.h"

int	create_window(t_man *man, const char *title, int width, int height)
{
	t_ivec2	monitor_size;

	man->mlx = mlx_init();
	if (!man->mlx)
		return (put_error(0, E_FAIL_MLX, 0));
	man->title = (char *)title;
	set_ivec2(&man->cursor, -1, -1);
	mlx_get_screen_size(man->mlx, &monitor_size.x, &monitor_size.y);
	set_resolution(man, monitor_size, width, height);
	man->window = mlx_new_window(man->mlx, man->res.window_size.x,
			man->res.window_size.y, man->title);
	set_viewport(man, man->res.window_size);
	if (!man->window)
		return (put_error(man, E_FAIL_WINDOW, 0));
	return (1);
}
