#include "cimmerian.h"

int	cross_window_button_callback(t_man *man)
{
	deinit(man);
	exit(0);
	return (0);
}

int	mouse_callback(int button, int x, int y, t_man *man)
{
	(void)x;
	(void)y;
	if (button == BTN_CLICK_LEFT)
	{
		//TODO
	}
	else if (button == BTN_CLICK_RIGHT)
	{
		//TODO
	}
	else if (button == BTN_SCROLL_UP)
		increase_minimap_zoom(man);
	else if (button == BTN_SCROLL_DOWN)
		decrease_minimap_zoom(man);
	return (0);
}

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
