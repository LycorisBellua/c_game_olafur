#include "olafur.h"

static void	size_for_fullscreen_toggle(GLFWwindow *window, int x, int y);
static void	pos_for_fullscreen_toggle(GLFWwindow *window, int x, int y);

void	framebuffer_size_callback(GLFWwindow *window, int x, int y)
{
	t_ivec2	framebuffer_size;

	set_ivec2(&framebuffer_size, x, y);
	set_viewport(&g_man, framebuffer_size);
	glViewport(g_man.res.viewport_offset.x, g_man.res.viewport_offset.y,
		g_man.res.viewport_size.x, g_man.res.viewport_size.y);
	size_for_fullscreen_toggle(window, x, y);
	return ;
}

void	window_pos_callback(GLFWwindow *window, int xpos, int ypos)
{
	pos_for_fullscreen_toggle(window, xpos, ypos);
	return ;
}

void	close_window_callback(t_man *man)
{
	glfwSetWindowShouldClose(man->window, GLFW_TRUE);
	return ;
}

static void	size_for_fullscreen_toggle(GLFWwindow *window, int x, int y)
{
	if (glfwGetWindowAttrib(window, GLFW_DECORATED))
		set_ivec2(&g_man.res.window_size, x, y);
	return ;
}

static void	pos_for_fullscreen_toggle(GLFWwindow *window, int x, int y)
{
	if (glfwGetWindowAttrib(window, GLFW_DECORATED))
		set_ivec2(&g_man.res.window_position, x, y);
	return ;
}
