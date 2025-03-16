#include "cimmerian.h"

static void	set_aspect_ratio(t_man *man);

void	set_resolution(t_man *man, t_ivec2 monitor_size, int width, int height)
{
	(void)height;
	man->res.monitor_size.x = monitor_size.x;
	man->res.monitor_size.y = monitor_size.y;
	set_aspect_ratio(man);
	man->res.window_size_default.x = min(width, man->res.monitor_size.x);
	man->res.window_size_default.y = min(width / man->res.aspect_ratio,
			man->res.monitor_size.y);
	man->res.window_size.x = man->res.window_size_default.x;
	man->res.window_size.y = man->res.window_size_default.y;
	man->res.window_position.x = 0;
	man->res.window_position.y = 0;
	man->res.fullscreen.x = (monitor_size.x - man->res.monitor_size.x) / 2;
	man->res.fullscreen.y = (monitor_size.y - man->res.monitor_size.y) / 2;
	return ;
}

void	set_viewport(t_man *man, t_ivec2 framebuffer_size)
{
	man->res.viewport_size.x = framebuffer_size.x;
	man->res.viewport_size.y = framebuffer_size.x / man->res.aspect_ratio;
	if (man->res.viewport_size.y > framebuffer_size.y)
	{
		man->res.viewport_size.y = framebuffer_size.y;
		man->res.viewport_size.x = framebuffer_size.y * man->res.aspect_ratio;
	}
	man->res.viewport_offset.x = (framebuffer_size.x
			- man->res.viewport_size.x) / 2;
	man->res.viewport_offset.y = (framebuffer_size.y
			- man->res.viewport_size.y) / 2;
	return ;
}

/*
	The if-statement is for monitors that are horizontally very long.
	`h_mod` is a height modifier for raycasting rendering.
*/
static void	set_aspect_ratio(t_man *man)
{
	man->res.aspect_ratio = (double)man->res.monitor_size.x
		/ man->res.monitor_size.y;
	if (man->res.aspect_ratio > 16.0 / 9)
	{
		man->res.aspect_ratio = 16.0 / 9;
		man->res.monitor_size.x = man->res.monitor_size.y
			* man->res.aspect_ratio;
	}
	man->res.h_mod = man->res.aspect_ratio
		- (round_f(man->res.aspect_ratio) - man->res.aspect_ratio);
	return ;
}
