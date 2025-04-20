#include "cimmerian.h"

void	physical_key_callback(GLFWwindow *window, int key, int scancode,
			int action, int mods);
void	scroll_callback(GLFWwindow *window, double x_offset, double y_offset);
void	mouse_callback(GLFWwindow *window, int button, int action, int mods);
void	cursor_pos_callback(GLFWwindow *window, double xpos, double ypos);

/*
	By default, the window closing event is only triggered by the cross window 
	button or Alt+F4. We also add ESC.
*/

void	init_input_handling(t_man *man)
{
	init_keys_wasdqe(man);
	init_keys_arrows(man);
	init_keys_misc(man);
	glfwSetKeyCallback(man->window, physical_key_callback);
	glfwSetInputMode(man->window, GLFW_LOCK_KEY_MODS, GLFW_TRUE);
	glfwSetScrollCallback(man->window, scroll_callback);
	glfwSetMouseButtonCallback(man->window, mouse_callback);
	glfwSetCursorPosCallback(man->window, cursor_pos_callback);
	set_ivec2(&man->cursor, -1, -1);
	return ;
}

void	poll_input_events(t_man *man)
{
	(void)man;
	glfwPollEvents();
	return ;
}
