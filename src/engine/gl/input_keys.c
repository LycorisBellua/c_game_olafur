#include "cimmerian.h"

void	physical_key_callback(GLFWwindow *window, int key, int scancode,
	int action, int mods)
{
	(void)window;
	(void)scancode;
	if (mods & GLFW_MOD_SHIFT)
		execute_input_action(&g_man, KEY_SHIFT, KEY_STATE_PRESSED);
	else
		execute_input_action(&g_man, KEY_SHIFT, KEY_STATE_RELEASED);
	if (action == GLFW_PRESS)
	{
		konami_code(&g_man, key);
		execute_input_action(&g_man, key, KEY_STATE_PRESSED);
	}
	else if (action == GLFW_REPEAT)
		execute_input_action(&g_man, key, KEY_STATE_HELD);
	else if (action == GLFW_RELEASE)
		execute_input_action(&g_man, key, KEY_STATE_RELEASED);
	return ;
}
