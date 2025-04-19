#include "cimmerian.h"

void	action_toggle_fullscreen(t_man *man)
{
	int					decorated;
	GLFWmonitor			*monitor;
	const GLFWvidmode	*mode;

	decorated = !glfwGetWindowAttrib(man->window, GLFW_DECORATED);
	if (decorated)
	{
		/* Switch to windowed mode */
		glfwSetWindowMonitor(man->window, NULL,
			man->res.window_position.x, man->res.window_position.y,
			man->res.window_size.x, man->res.window_size.y, GLFW_DONT_CARE);
	}
	else
	{
		/* Switch to fullscreen */
		monitor = glfwGetPrimaryMonitor();
		mode = glfwGetVideoMode(monitor);
		glfwSetWindowMonitor(man->window, monitor, man->res.fullscreen.x,
			man->res.fullscreen.y, man->res.monitor_size.x,
			man->res.monitor_size.y, mode->refreshRate);
	}
	glfwSetWindowAttrib(man->window, GLFW_DECORATED, decorated);
	return ;
}
