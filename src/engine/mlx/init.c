#include "cimmerian.h"

int	create_window(t_man *man, const char *title, int width, int height)
{
	man->mlx = mlx_init();
	if (!man->mlx)
		return (put_error(0, E_FAIL_MLX, 0));
	man->title = (char *)title;
	set_ivec2(&man->cursor, -1, -1);
	set_resolution(man, width, height);
	man->window = mlx_new_window(man->mlx, man->res.window_size.x,
			man->res.window_size.y, man->title);
	set_viewport(man, man->res.window_size);
	if (!man->window)
		return (put_error(man, E_FAIL_WINDOW, 0));
	return (1);
}
