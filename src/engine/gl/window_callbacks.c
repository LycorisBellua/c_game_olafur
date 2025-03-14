#include "cimmerian.h"

void	framebuffer_size_callback(GLFWwindow *window, int x, int y)
{
	t_ivec2	framebuffer_size;

	(void)window;
	set_ivec2(&framebuffer_size, x, y);
	set_viewport(&g_man, framebuffer_size);
	glViewport(g_man.res.viewport_offset.x, g_man.res.viewport_offset.y,
		g_man.res.viewport_size.x, g_man.res.viewport_size.y);
	/* for toggle_fullscreen() */
	if (glfwGetWindowAttrib(window, GLFW_DECORATED))
		set_ivec2(&g_man.res.window_size, x, y);
	return ;
}

void	window_pos_callback(GLFWwindow *window, int xpos, int ypos)
{
	/* for toggle_fullscreen() */
	if (glfwGetWindowAttrib(window, GLFW_DECORATED))
		set_ivec2(&g_man.res.window_position, xpos, ypos);
	return ;
}
