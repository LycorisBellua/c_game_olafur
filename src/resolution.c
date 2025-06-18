#include "olafur.h"

static t_ivec2	adapt_size_to_ratio(t_man *man, t_ivec2 size);

int	set_resolution(t_man *man, t_ivec2 window_size, float aspect_ratio)
{
	man->res.monitor_size = get_monitor_size(man);
	man->res.window_size_default.x = clamp(window_size.x,
			128, man->res.monitor_size.x);
	man->res.window_size_default.y = clamp(window_size.y,
			128, man->res.monitor_size.y);
	man->res.window_size.x = man->res.window_size_default.x;
	man->res.window_size.y = man->res.window_size_default.y;
	set_ivec2(&man->res.window_position, 0, 0);
	man->res.half_fov_tan = tanf(deg2rad(53) / 2.0);
	man->res.ratio = aspect_ratio;
	if (aspect_ratio <= 0.1)
		man->res.ratio = (float)man->res.monitor_size.x
			/ man->res.monitor_size.y;
	if (aspect_ratio <= 0.1)
		return (0);
	man->res.res = adapt_size_to_ratio(man, man->res.window_size_default);
	return (1);
}

void	set_viewport(t_man *man, t_ivec2 window_size)
{
	man->res.viewport_size = adapt_size_to_ratio(man, window_size);
	man->res.viewport_offset.x = (window_size.x - man->res.viewport_size.x)
		/ 2;
	man->res.viewport_offset.y = (window_size.y - man->res.viewport_size.y)
		/ 2;
	return ;
}

static t_ivec2	adapt_size_to_ratio(t_man *man, t_ivec2 size)
{
	t_ivec2	adapted;

	adapted.x = size.x;
	adapted.y = (int)(adapted.x / man->res.ratio);
	if (adapted.y > size.y)
	{
		adapted.y = size.y;
		adapted.x = (int)(adapted.y * man->res.ratio);
	}
	return (adapted);
}
