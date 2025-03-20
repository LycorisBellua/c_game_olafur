#include "cimmerian.h"

int	mouse_press_callback(int button, int x, int y, t_man *man)
{
	(void)x;
	(void)y;
	if (button == BTN_CLICK_LEFT)
		man->l_click_action = 1;
	else if (button == BTN_CLICK_RIGHT)
		man->r_click_action = 1;
	else if (man->game_state == GAME_STATE_PLAY)
	{
		if (button == BTN_SCROLL_UP)
			increase_minimap_zoom(man);
		else if (button == BTN_SCROLL_DOWN)
			decrease_minimap_zoom(man);
	}
	return (0);
}

int	mouse_release_callback(int button, int x, int y, t_man *man)
{
	(void)x;
	(void)y;
	if (button == BTN_CLICK_LEFT)
		man->l_click_action = 0;
	else if (button == BTN_CLICK_RIGHT)
		man->r_click_action = 0;
	return (0);
}
