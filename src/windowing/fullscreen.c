#include "olafur.h"

static void	enter_fullscreen(t_man *man);
static void	leave_fullscreen(t_man *man);

void	toggle_fullscreen(t_man *man)
{
	if (!man->res.is_fullscreen)
		enter_fullscreen(man);
	else
		leave_fullscreen(man);
	return ;
}

void	set_monitor_size(t_man *man)
{
	t_ivec2				size;
	const GLFWvidmode	*mode;

	set_ivec2(&size, 0, 0);
	mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	if (mode)
		set_ivec2(&size, mode->width, mode->height);
	man->res.monitor_size = size;
	return ;
}

static void	enter_fullscreen(t_man *man)
{
	GLFWmonitor			*monitor;
	const GLFWvidmode	*mode;

	glfwGetWindowPos(man->window, &man->res.window_pos.x,
		&man->res.window_pos.y);
	glfwGetWindowSize(man->window, &man->res.window_size.x,
		&man->res.window_size.y);
	monitor = glfwGetPrimaryMonitor();
	mode = glfwGetVideoMode(monitor);
	if (!mode)
		return ;
	glfwSetWindowMonitor(man->window, monitor,
		0, 0, mode->width, mode->height, mode->refreshRate);
	man->res.is_fullscreen = 1;
	return ;
}

static void	leave_fullscreen(t_man *man)
{
	glfwSetWindowMonitor(man->window, 0,
		man->res.window_pos.x, man->res.window_pos.y,
		man->res.window_size.x, man->res.window_size.y, GLFW_DONT_CARE);
	glfwFocusWindow(man->window);
	man->res.is_fullscreen = 0;
	return ;
}
