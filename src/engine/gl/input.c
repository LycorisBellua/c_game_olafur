#include "cimmerian.h"

void	physical_key_callback(GLFWwindow *window, int key, int scancode,
			int action, int mods);
void	scroll_callback(GLFWwindow *window, double x_offset, double y_offset);
void	mouse_callback(GLFWwindow *window, int button, int action, int mods);
void	cursor_pos_callback(GLFWwindow *window, double xpos, double ypos);

void	init_input_handling(t_man *man)
{
	glfwSetKeyCallback(man->window, physical_key_callback);
	glfwSetInputMode(man->window, GLFW_LOCK_KEY_MODS, GLFW_TRUE);
	glfwSetScrollCallback(man->window, scroll_callback);
	glfwSetMouseButtonCallback(man->window, mouse_callback);
	glfwSetCursorPosCallback(man->window, cursor_pos_callback);
	set_ivec2(&man->cursor, -1, -1);
	return ;
}

void	handle_input(t_man *man)
{
	glfwPollEvents();
	update_player_transform(man);
	return ;
}
