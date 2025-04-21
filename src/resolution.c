#include "cimmerian.h"

static t_ivec2	adapt_size_to_ratio(t_man *man, t_ivec2 size);

/*
	`h_mod` is the "height modifier" for raycasting rendering so that walls are 
	square, instead of matching the aspect ratio.
*/
void	set_resolution(t_man *man, int window_width, int window_height)
{
	man->res.monitor_size = get_monitor_size(man);
	man->res.window_size_default.x = clamp(window_width,
			1, man->res.monitor_size.x);
	man->res.window_size_default.y = clamp(window_height,
			1, man->res.monitor_size.y);
	man->res.window_size.x = man->res.window_size_default.x;
	man->res.window_size.y = man->res.window_size_default.y;
	set_ivec2(&man->res.window_position, 0, 0);
	man->res.ratio = (double)man->res.monitor_size.x / man->res.monitor_size.y;
	man->res.h_mod = man->res.ratio - (round(man->res.ratio) - man->res.ratio);
	man->res.res = adapt_size_to_ratio(man, man->res.window_size_default);
	return ;
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

	if (man->res.monitor_size.x >= man->res.monitor_size.y)
	{
		adapted.x = man->res.monitor_size.x
			/ round_up_to_half((double)man->res.monitor_size.x / size.x);
		adapted.y = adapted.x / man->res.ratio;
		if (adapted.y > size.y)
		{
			adapted.y = man->res.monitor_size.y
				/ round_up_to_half((double)man->res.monitor_size.y / size.y);
			adapted.x = adapted.y * man->res.ratio;
		}
		return (adapted);
	}
	adapted.y = man->res.monitor_size.y
		/ round_up_to_half((double)man->res.monitor_size.y / size.y);
	adapted.x = adapted.y / man->res.ratio;
	if (adapted.x > size.x)
	{
		adapted.x = man->res.monitor_size.x
			/ round_up_to_half((double)man->res.monitor_size.x / size.x);
		adapted.y = adapted.x * man->res.ratio;
	}
	return (adapted);
}
