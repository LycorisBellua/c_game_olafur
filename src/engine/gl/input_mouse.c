#include "cimmerian.h"

static void	rotate_on_click(t_man *man);
static void	init_rotate_click(t_man *man, int *x_first_click, int *x_max_diff);
static int	get_diff_perc(t_man *man, int diff);

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
			g_man.r_click_action = 1;
		else if (action == GLFW_RELEASE)
			g_man.r_click_action = 0;
	}
	return ;
}

void	cursor_pos_callback(GLFWwindow *window, double xpos, double ypos)
{
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
	rotate_on_click(&g_man);
	return ;
}

static void	rotate_on_click(t_man *man)
{
	static int	x_first_click;
	static int	x_max_diff;
	int			diff;

	if (!man->r_click_action)
		x_first_click = -1;
	else if (x_first_click < 0)
		init_rotate_click(man, &x_first_click, &x_max_diff);
	if (x_first_click >= 0)
	{
		diff = man->cursor.x - x_first_click;
		if ((diff > 0 && diff > x_max_diff) || (diff < 0 && diff < x_max_diff))
			x_max_diff = diff;
		if ((diff > 0 && diff < x_max_diff) || (diff < 0 && diff > x_max_diff))
		{
			x_first_click += x_max_diff;
			diff = man->cursor.x - x_first_click;
			x_max_diff = diff;
		}
		rotate_player(man,
			RAD_1 * get_diff_perc(man, diff) * norm(diff) * man->dt);
	}
	return ;
}

static void	init_rotate_click(t_man *man, int *x_first_click, int *x_max_diff)
{
	*x_first_click = man->cursor.x;
	*x_max_diff = 0;
	return ;
}

static int	get_diff_perc(t_man *man, int diff)
{
	return (max(10, abs(diff) * 100 / man->res.window_size_default.x));
}
