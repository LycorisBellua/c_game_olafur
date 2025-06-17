#include "olafur.h"

static void	rendering(t_man *man, t_map *map);

void	game_loop(t_man *man)
{
	t_map	*map;

	map = man->maps[man->curr_map];
	set_dt_and_fps(man);
	advance_all_image_cycles(man);
	audio_source_delete_marked(map);
	rendering(man, map);
	if (man->game_state == GAME_STATE_PLAY)
	{
		update_player_transform(man);
		door_routine(man);
		portal_routine(man);
	}
	return ;
}

static void	rendering(t_man *man, t_map *map)
{
	if (man->game_state == GAME_STATE_PLAY)
	{
		if (man->echolocation || map->skybox->cycle_time_in_ms)
			compose_background(man, map);
		draw_png_with_x_offset(man, map->background, map->background_offset);
		raycasting(man);
	}
	display_game_gui(man, map);
	display_frame(man);
	return ;
}
