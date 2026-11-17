#include "olafur.h"

static void	focus_callback(GLFWwindow *window, int focused);

void	init_input_handling(t_man *man)
{
	set_ivec2(&man->cursor, -1, -1);
	init_input_keys(man);
	glfwSetKeyCallback(man->window, physical_key_callback);
	glfwSetInputMode(man->window, GLFW_LOCK_KEY_MODS, GLFW_TRUE);
	glfwSetScrollCallback(man->window, scroll_callback);
	glfwSetMouseButtonCallback(man->window, mouse_callback);
	glfwSetCursorPosCallback(man->window, cursor_pos_callback);
	glfwSetWindowFocusCallback(man->window, focus_callback);
	return ;
}

static void	focus_callback(GLFWwindow *window, int focused)
{
	(void)window;
	if (!focused)
	{
		cancel_keys_in_game_state(&g_man);
		g_man.l_click_action = 0;
		g_man.r_click_action = 0;
	}
	return ;
}
