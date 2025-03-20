#include "cimmerian.h"

void	render_game(t_man *man, t_frame *f)
{
	update_background(man->map);
	draw_background(man, f);
	raycasting(man, f);
	display_game_gui(man, f);
	return ;
}
