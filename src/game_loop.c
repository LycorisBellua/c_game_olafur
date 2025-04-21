#include "cimmerian.h"

static void	rendering(t_man *man);

int	game_loop(t_man *man)
{
	set_dt_and_fps(man);
	advance_all_image_cycles(man);
	rendering(man);
	if (man->game_state == GAME_STATE_PLAY)
	{
		update_player_transform(man);
		door_routine(man);
		portal_routine(man);
	}
	poll_input_events(man);
	return (0);
}

static void	rendering(t_man *man)
{
	t_map	*map;

	clear_frame(man);
	if (man->game_state != GAME_STATE_PLAY)
		display_game_over_screen(man);
	else
	{
		map = man->maps[man->curr_map];
		if (man->echolocation || map->skybox->cycle_time_in_ms)
			compose_background(man, map);
		draw_png_with_x_offset(man, map->background, map->background_offset);
		raycasting(man);
		display_game_gui(man);
	}
	display_frame(man);
	return ;
}
