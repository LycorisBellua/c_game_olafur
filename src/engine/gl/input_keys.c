#include "cimmerian.h"

static void	handle_key_press(t_man *man, int key, int mods);
static void	handle_key_release(t_man *man, int key, int mods);

/*
	By default, the window closing event is only triggered by the cross window 
	button or Alt+F4.
*/

void	physical_key_callback(GLFWwindow *window, int key, int scancode,
	int action, int mods)
{
	(void)window;
	(void)scancode;
	if (g_man.game_state == GAME_STATE_PLAY)
		handle_player_speed(&g_man, mods & GLFW_MOD_SHIFT);
	if (action == GLFW_PRESS)
		handle_key_press(&g_man, key, mods);
	else if (action == GLFW_RELEASE)
		handle_key_release(&g_man, key, mods);
	return ;
}

static void	handle_key_press(t_man *man, int key, int mods)
{
	if (key == GLFW_KEY_ESCAPE)
		glfwSetWindowShouldClose(man->window, GLFW_TRUE);
	else if (key == GLFW_KEY_F11)
		toggle_fullscreen(man);
	else if (man->game_state == GAME_STATE_PLAY)
	{
		if (key == GLFW_KEY_DOWN || key == GLFW_KEY_S
			|| (!(mods & GLFW_MOD_NUM_LOCK) && key == GLFW_KEY_KP_2))
			man->move_action.y += -1;
		else if (key == GLFW_KEY_UP || key == GLFW_KEY_W
			|| (!(mods & GLFW_MOD_NUM_LOCK) && key == GLFW_KEY_KP_8))
			man->move_action.y += 1;
		else if (key == GLFW_KEY_Q)
			man->move_action.x += -1;
		else if (key == GLFW_KEY_E)
			man->move_action.x += 1;
		else if (key == GLFW_KEY_LEFT || key == GLFW_KEY_A
			|| (!(mods & GLFW_MOD_NUM_LOCK) && key == GLFW_KEY_KP_4))
			man->rotate_action += -1;
		else if (key == GLFW_KEY_RIGHT || key == GLFW_KEY_D
			|| (!(mods & GLFW_MOD_NUM_LOCK) && key == GLFW_KEY_KP_6))
			man->rotate_action += 1;
	}
	return ;
}

static void	handle_key_release(t_man *man, int key, int mods)
{
	if (man->game_state == GAME_STATE_PLAY)
	{
		if (key == GLFW_KEY_DOWN || key == GLFW_KEY_S
			|| (!(mods & GLFW_MOD_NUM_LOCK) && key == GLFW_KEY_KP_2))
			man->move_action.y += 1;
		else if (key == GLFW_KEY_UP || key == GLFW_KEY_W
			|| (!(mods & GLFW_MOD_NUM_LOCK) && key == GLFW_KEY_KP_8))
			man->move_action.y += -1;
		else if (key == GLFW_KEY_Q)
			man->move_action.x += 1;
		else if (key == GLFW_KEY_E)
			man->move_action.x += -1;
		else if (key == GLFW_KEY_LEFT || key == GLFW_KEY_A
			|| (!(mods & GLFW_MOD_NUM_LOCK) && key == GLFW_KEY_KP_4))
			man->rotate_action += 1;
		else if (key == GLFW_KEY_RIGHT || key == GLFW_KEY_D
			|| (!(mods & GLFW_MOD_NUM_LOCK) && key == GLFW_KEY_KP_6))
			man->rotate_action += -1;
	}
	return ;
}

/*
// It will be in lowercase, so -32 for uppercase
int	get_local_key(int physical_key)
{
	return (glfwGetKeyName(physical_key, 0)[0] - 32);
}
*/
