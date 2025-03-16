#include "cimmerian.h"

int	cross_window_button_callback(t_man *man);

int	key_press_callback(int key, t_man *man)
{
	if (key == KEY_SHIFT)
		handle_player_speed(man, 1);
	else if (key == KEY_ESCAPE)
		cross_window_button_callback(man);
	else if (key == KEY_F11)
		toggle_fullscreen(man);
	else if (key == KEY_DOWN || key == KEY_S)
		man->move_action.y += -1;
	else if (key == KEY_UP || key == KEY_W)
		man->move_action.y += 1;
	else if (key == KEY_Q)
		man->move_action.x += -1;
	else if (key == KEY_E)
		man->move_action.x += 1;
	else if (key == KEY_LEFT || key == KEY_A)
		man->rotate_action += -1;
	else if (key == KEY_RIGHT || key == KEY_D)
		man->rotate_action += 1;
	return (0);
}

int	key_release_callback(int key, t_man *man)
{
	if (key == KEY_SHIFT)
		handle_player_speed(man, 0);
	else if (key == KEY_DOWN || key == KEY_S)
		man->move_action.y += 1;
	else if (key == KEY_UP || key == KEY_W)
		man->move_action.y += -1;
	else if (key == KEY_Q)
		man->move_action.x += 1;
	else if (key == KEY_E)
		man->move_action.x += -1;
	else if (key == KEY_LEFT || key == KEY_A)
		man->rotate_action += 1;
	else if (key == KEY_RIGHT || key == KEY_D)
		man->rotate_action += -1;
	return (0);
}
