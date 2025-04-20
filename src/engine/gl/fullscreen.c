#include "cimmerian.h"

static void	switch_to_windowed_mode(t_man *man);
static void	switch_to_fullscreen_mode(t_man *man);

void	toggle_fullscreen(t_man *man)
{
	int	decorated;

	decorated = !glfwGetWindowAttrib(man->window, GLFW_DECORATED);
	if (decorated)
		switch_to_windowed_mode(man);
	else
		switch_to_fullscreen_mode(man);
	glfwSetWindowAttrib(man->window, GLFW_DECORATED, decorated);
	return ;
}

static void	switch_to_windowed_mode(t_man *man)
{
	glfwSetWindowMonitor(man->window, NULL,
		man->res.window_position.x, man->res.window_position.y,
		man->res.window_size.x, man->res.window_size.y, GLFW_DONT_CARE);
	return ;
}

static void	switch_to_fullscreen_mode(t_man *man)
{
	GLFWmonitor			*monitor;
	const GLFWvidmode	*mode;

	monitor = glfwGetPrimaryMonitor();
	mode = glfwGetVideoMode(monitor);
	glfwSetWindowMonitor(man->window, monitor, man->res.fullscreen.x,
		man->res.fullscreen.y, man->res.monitor_size.x,
		man->res.monitor_size.y, mode->refreshRate);
	return ;
}
