#include "olafur.h"

int	create_window(t_man *man, const char *title, t_ivec2 size,
	float aspect_ratio)
{
	man->xvar = sua_init();
	if (!man->xvar)
		return (put_error(0, E_FAIL_SUA, 0, 0));
	if (!set_resolution(man, size, aspect_ratio))
		return (put_error(0, E_FAIL_WINDOW, 0, 0));
	man->title = (char *)title;
	if (sua_window_create(man->xvar, man->res.window_size.x,
			man->res.window_size.y, man->title) < 0)
		return (put_error(man, E_FAIL_WINDOW, 0, 0));
	set_viewport(man, man->res.window_size);
	return (1);
}
