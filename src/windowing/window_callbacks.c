#include "olafur.h"

void	framebuffer_size_callback(GLFWwindow *window, int x, int y)
{
	t_ivec2	framebuffer_size;

	(void)window;
	if (!x || !y)
		return ;
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

int	window_pos_to_view(t_man *man, double xpos, double ypos, t_ivec2 *pos)
{
	t_ivec2	win;
	t_ivec2	fb;
	t_ivec2	*size;

	set_ivec2(pos, -1, -1);
	size = &man->res.viewport_size;
	glfwGetWindowSize(man->window, &win.x, &win.y);
	glfwGetFramebufferSize(man->window, &fb.x, &fb.y);
	if (!win.x || !win.y || !size->x || !size->y)
		return (0);
	set_ivec2(pos, (int)((float)xpos * fb.x / win.x)
		- man->res.viewport_offset.x, (int)((float)ypos * fb.y / win.y)
		- man->res.viewport_offset.y);
	if (pos->x < 0 || pos->y < 0 || pos->x >= size->x || pos->y >= size->y)
	{
		set_ivec2(pos, -1, -1);
		return (0);
	}
	pos->x = pos->x * man->res.res.x / size->x;
	pos->y = pos->y * man->res.res.y / size->y;
	return (1);
}
