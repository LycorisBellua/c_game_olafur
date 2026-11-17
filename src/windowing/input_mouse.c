#include "olafur.h"

static void	start_drag_rotation(t_man *man);
static void	update_drag_rotation(t_man *man);

void	mouse_callback(GLFWwindow *window, int button, int action, int mods)
{
	(void)window;
	(void)mods;
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS)
			g_man.l_click_action = 1;
		else if (action == GLFW_RELEASE)
			g_man.l_click_action = 0;
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		if (action == GLFW_PRESS)
			start_drag_rotation(&g_man);
		else if (action == GLFW_RELEASE)
			g_man.r_click_action = 0;
	}
	return ;
}

void	cursor_pos_callback(GLFWwindow *window, double xpos, double ypos)
{
	if (!window_pos_to_view(&g_man, xpos, ypos, &g_man.cursor))
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	else if (g_man.img_cursor)
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	update_drag_rotation(&g_man);
	return ;
}

static void	start_drag_rotation(t_man *man)
{
	man->r_click_action = 1;
	man->drag_anchor_x = -1;
	man->drag_max_diff = 0;
	man->drag_rotate_speed = 0;
	return ;
}

static void	update_drag_rotation(t_man *man)
{
	int	diff;
	int	perc;

	if (!man->r_click_action || man->cursor.x < 0)
		return ;
	if (man->drag_anchor_x < 0)
		man->drag_anchor_x = man->cursor.x;
	diff = man->cursor.x - man->drag_anchor_x;
	if ((diff > 0 && diff > man->drag_max_diff)
		|| (diff < 0 && diff < man->drag_max_diff))
		man->drag_max_diff = diff;
	if ((diff > 0 && diff < man->drag_max_diff)
		|| (diff < 0 && diff > man->drag_max_diff))
	{
		man->drag_anchor_x += man->drag_max_diff;
		diff = man->cursor.x - man->drag_anchor_x;
		man->drag_max_diff = diff;
	}
	perc = imax(10, abs(diff) * 100 / man->res.res.x);
	man->drag_rotate_speed = DRAG_ROTATE_SPEED * RAD_1 * perc * norm(diff);
	return ;
}
