#include "cimmerian.h"

void	run_game_loop(t_map *m)
{
	t_frame	*f;

	f = g_man.frame[g_man.curr_frame];
	draw_background(f, m);
	raycasting(m);
	door_routine(m);
	/*
		- 60 FPS. Objective is 150 FPS.
		- Image resolution doesn't do anything.
	*/
	return ;
}
