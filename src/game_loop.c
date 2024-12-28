#include "cimmerian.h"

static void	display_gui(t_frame *f);

void	run_game_loop(t_map *m)
{
	t_frame	*f;

	f = g_man.frame[g_man.curr_frame];
	draw_background(f, m);
	raycasting(f, m);
	door_routine(m);
	display_gui(f);
	/*
		- 60 FPS. Objective is 150 FPS.
		- A lower image resolution or even flat colors don't change anything.
	*/
	return ;
}

static void	display_gui(t_frame *f)
{
	t_ivec2	pos;

	set_ivec2(&pos, 0, 0);
	draw_sprite(f, g_man.sprites + 2, pos, g_man.dt_ms);
	pos.x += g_man.sprites[2].size.x;
	pos.y += 14;
	display_fps(f, pos);
	if (g_man.cursor.x >= 0 && g_man.cursor.y >= 0)
		draw_cursor(f, &g_man.sprites[0], g_man.cursor, g_man.click_action);
	return ;
}
