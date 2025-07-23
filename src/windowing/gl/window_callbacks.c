#include "olafur.h"

void	framebuffer_size_callback(GLFWwindow *window, int x, int y)
{
	t_ivec2	framebuffer_size;

	(void)window;
	set_ivec2(&framebuffer_size, x, y);
	set_viewport(&g_man, framebuffer_size);
	glViewport(g_man.res.viewport_offset.x, g_man.res.viewport_offset.y,
		g_man.res.viewport_size.x, g_man.res.viewport_size.y);
	return ;
}

void	close_window_callback(t_man *man)
{
	glfwSetWindowShouldClose(man->window, GLFW_TRUE);
	return ;
}
