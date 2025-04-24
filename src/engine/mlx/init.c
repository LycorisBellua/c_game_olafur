#include "cimmerian.h"

int	create_window(t_man *man, const char *title, t_ivec2 size,
	double aspect_ratio)
{
	man->mlx = mlx_init();
	if (!man->mlx)
		return (put_error(0, E_FAIL_MLX, 0));
	if (!set_resolution(man, size, aspect_ratio))
		return (put_error(0, E_FAIL_WINDOW, 0));
	man->title = (char *)title;
	man->window = mlx_window_create(man->mlx, man->res.window_size.x,
			man->res.window_size.y, man->title);
	set_viewport(man, man->res.window_size);
	if (!man->window)
		return (put_error(man, E_FAIL_WINDOW, 0));
	return (1);
}
