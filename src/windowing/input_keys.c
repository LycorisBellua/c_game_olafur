#include "olafur.h"

static int	normalize_keypad_keys(int num_lock, int key);

void	physical_key_callback(GLFWwindow *window, int key, int scancode,
	int action, int mods)
{
	(void)window;
	(void)scancode;
	key = normalize_keypad_keys(mods & GLFW_MOD_NUM_LOCK, key);
	if (key == GLFW_KEY_UNKNOWN)
		return ;
	else if (action == GLFW_PRESS)
	{
		teleport_through_key_sequence(&g_man, key);
		execute_input_action(&g_man, key, KEY_STATE_PRESSED);
	}
	else if (action == GLFW_REPEAT)
		execute_input_action(&g_man, key, KEY_STATE_HELD);
	else if (action == GLFW_RELEASE)
		execute_input_action(&g_man, key, KEY_STATE_RELEASED);
	return ;
}

static int	normalize_keypad_keys(int num_lock, int key)
{
	/*
		Ignore the action of the Num Lock key itself being pressed, otherwise 
		it would interrupt a TP sequence when the user wants to use the key pad 
		for both digits and arrows.
	*/
	if (key == GLFW_KEY_NUM_LOCK)
		return (GLFW_KEY_UNKNOWN);
	else if (key == GLFW_KEY_KP_ENTER)
		key = GLFW_KEY_ENTER;
	else if (key >= GLFW_KEY_KP_0 && key <= GLFW_KEY_KP_9)
	{
		if (num_lock)
			key = GLFW_KEY_0 + (key - GLFW_KEY_KP_0);
		else if (key == GLFW_KEY_KP_8)
			key = GLFW_KEY_UP;
		else if (key == GLFW_KEY_KP_2)
			key = GLFW_KEY_DOWN;
		else if (key == GLFW_KEY_KP_4)
			key = GLFW_KEY_LEFT;
		else if (key == GLFW_KEY_KP_6)
			key = GLFW_KEY_RIGHT;
	}
	return (key);
}
