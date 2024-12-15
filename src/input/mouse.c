#include "cimmerian.h"

void	mouse_callback(GLFWwindow *window, int button, int action, int mods)
{
	(void)window;
	(void)mods;
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS)
			g_man.click_action = 1;
		else if (action == GLFW_RELEASE)
			g_man.click_action = 0;
	}
	return ;
}

void	cursor_pos_callback(GLFWwindow *window, double xpos, double ypos)
{
	/*
	if (g_man.cursor.x >= 0 && g_man.cursor.y >= 0)
		draw_cursor(frame, &g_man.sprites[1], g_man.cursor, g_man.click_action);
	*/
	g_man.cursor.x = ((int)xpos - g_man.res.viewport_offset.x)
		* g_man.res.window_size_default.x / g_man.res.viewport_size.x;
	g_man.cursor.y = ((int)ypos - g_man.res.viewport_offset.y)
		* g_man.res.window_size_default.y / g_man.res.viewport_size.y;
	if (g_man.cursor.x < 0
		|| g_man.cursor.x >= g_man.res.window_size_default.x
		|| g_man.cursor.y < 0
		|| g_man.cursor.y >= g_man.res.window_size_default.y)
	{
		set_ivec2(&g_man.cursor, -1, -1);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		return ;
	}
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	return ;
}
