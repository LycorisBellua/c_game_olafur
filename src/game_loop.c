#include "cimmerian.h"

void	run_game_loop(t_map *m)
{
	t_frame	*f;

	f = g_man.frame[g_man.curr_frame];
	draw_skybox(f, m->img[0], m->fog_width, m->fog_color);
	draw_floor(f, m->fog_width, m->fog_color);
	raycasting(m);
	door_routine(m);
	return ;
}
