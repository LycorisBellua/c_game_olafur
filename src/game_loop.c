#include "cimmerian.h"

static void	rendering(t_man *man, t_frame *f);

int	game_loop(t_man *man)
{
	t_frame	*f;

	f = man->frame + man->curr_frame;
	set_dt_and_fps(man);
	advance_all_image_cycles(man);
	rendering(man, f);
	if (man->game_state == GAME_STATE_PLAY)
	{
		update_player_transform(man);
		door_routine(man);
		portal_routine(man);
	}
	poll_input_events(man);
	return (0);
}

static void	rendering(t_man *man, t_frame *f)
{
	t_map	*map;

	clear_frame(f);
	if (man->game_state != GAME_STATE_PLAY)
		display_game_over_screen(man, f);
	else
	{
		map = man->maps[man->curr_map];
		if (man->echolocation || map->skybox->cycle_time_in_ms)
			compose_background(man, map);
		draw_png_with_x_offset(f, map->background, map->background_offset);
		raycasting(man, f);
		display_game_gui(man, f);
	}
	display_frame(man, f);
	man->curr_frame = (man->curr_frame + 1) % 2;
	return ;
}
