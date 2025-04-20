#include "cimmerian.h"

void	action_toggle_fullscreen(t_man *man)
{
	toggle_fullscreen(man);
	return ;
}

void	action_toggle_debug(t_man *man)
{
	man->show_debug = !man->show_debug;
	return ;
}

void	action_close_window(t_man *man)
{
	cross_window_button_callback(man);
	return ;
}
