#include "olafur.h"

void	toggle_fullscreen(t_man *man)
{
	GLFWmonitor			*monitor;
	const GLFWvidmode	*mode;

	if (!man->res.is_fullscreen)
	{
		glfwGetWindowPos(man->window, &man->res.window_pos.x,
			&man->res.window_pos.y);
		glfwGetWindowSize(man->window, &man->res.window_size.x,
			&man->res.window_size.y);
		monitor = glfwGetPrimaryMonitor();
		mode = glfwGetVideoMode(monitor);
		glfwSetWindowMonitor(man->window, monitor,
			man->res.fullscreen.x, man->res.fullscreen.y,
			mode->width, mode->height, mode->refreshRate);
		man->res.is_fullscreen = 1;
	}
	else
	{
		glfwSetWindowMonitor(man->window, 0,
			man->res.window_pos.x, man->res.window_pos.y,
			man->res.window_size.x, man->res.window_size.y, GLFW_DONT_CARE);
		glfwFocusWindow(man->window);
		man->res.is_fullscreen = 0;
	}
	return ;
}

void	set_monitor_size(t_man *man)
{
	t_ivec2				size;
	const GLFWvidmode	*mode;

	(void)man;
	set_ivec2(&size, 0, 0);
	mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	if (mode)
		set_ivec2(&size, mode->width, mode->height);
	man->res.monitor_size = size;
	man->res.fullscreen.x = (size.x - man->res.monitor_size.x) / 2;
	man->res.fullscreen.y = (size.y - man->res.monitor_size.y) / 2;
	return ;
}
