#include "cimmerian.h"

static void	handle_player_speed(int mods);
static void	handle_key_press(GLFWwindow *window, int key, int mods);
static void	handle_key_release(int key, int mods);

/*
	By default, the window closing event is only triggered by the cross window 
	button or Alt+F4.
*/

void	physical_key_callback(GLFWwindow *window, int key, int scancode,
	int action, int mods)
{
	(void)scancode;
	handle_player_speed(mods);
	if (action == GLFW_PRESS)
		handle_key_press(window, key, mods);
	else if (action == GLFW_RELEASE)
		handle_key_release(key, mods);
	return ;
}

static void	handle_player_speed(int mods)
{
	if (mods & GLFW_MOD_SHIFT)
	{
		g_man.movement_speed = DEFAULT_MOVE_SPEED * 2;
		g_man.rotation_speed = DEFAULT_ROT_SPEED * 2;
	}
	else
	{
		g_man.movement_speed = DEFAULT_MOVE_SPEED;
		g_man.rotation_speed = DEFAULT_ROT_SPEED;
	}
	return ;
}

static void	handle_key_press(GLFWwindow *window, int key, int mods)
{
	if (key == GLFW_KEY_ESCAPE)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	else if (key == GLFW_KEY_F11)
		toggle_fullscreen(window);
	else if (key == GLFW_KEY_DOWN || key == GLFW_KEY_S
		|| (!(mods & GLFW_MOD_NUM_LOCK) && key == GLFW_KEY_KP_2))
		g_man.movement_action.y += -1;
	else if (key == GLFW_KEY_UP || key == GLFW_KEY_W
		|| (!(mods & GLFW_MOD_NUM_LOCK) && key == GLFW_KEY_KP_8))
		g_man.movement_action.y += 1;
	else if (key == GLFW_KEY_Q)
		g_man.movement_action.x += -1;
	else if (key == GLFW_KEY_E)
		g_man.movement_action.x += 1;
	else if (key == GLFW_KEY_LEFT || key == GLFW_KEY_A
		|| (!(mods & GLFW_MOD_NUM_LOCK) && key == GLFW_KEY_KP_4))
		g_man.rotation_action += -1;
	else if (key == GLFW_KEY_RIGHT || key == GLFW_KEY_D
		|| (!(mods & GLFW_MOD_NUM_LOCK) && key == GLFW_KEY_KP_6))
		g_man.rotation_action += 1;
	return ;
}

static void	handle_key_release(int key, int mods)
{
	if (key == GLFW_KEY_DOWN || key == GLFW_KEY_S
		|| (!(mods & GLFW_MOD_NUM_LOCK) && key == GLFW_KEY_KP_2))
		g_man.movement_action.y += 1;
	else if (key == GLFW_KEY_UP || key == GLFW_KEY_W
		|| (!(mods & GLFW_MOD_NUM_LOCK) && key == GLFW_KEY_KP_8))
		g_man.movement_action.y += -1;
	else if (key == GLFW_KEY_Q)
		g_man.movement_action.x += 1;
	else if (key == GLFW_KEY_E)
		g_man.movement_action.x += -1;
	else if (key == GLFW_KEY_LEFT || key == GLFW_KEY_A
		|| (!(mods & GLFW_MOD_NUM_LOCK) && key == GLFW_KEY_KP_4))
		g_man.rotation_action += 1;
	else if (key == GLFW_KEY_RIGHT || key == GLFW_KEY_D
		|| (!(mods & GLFW_MOD_NUM_LOCK) && key == GLFW_KEY_KP_6))
		g_man.rotation_action += -1;
	return ;
}

/*
// It will be in lowercase, so -32 for uppercase
int	get_local_key(int physical_key)
{
	return (glfwGetKeyName(physical_key, 0)[0] - 32);
}
*/
