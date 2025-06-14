#include "cimmerian.h"

static int	init_game(t_man *man, const char *map_path);

t_man	g_man;

int	main(int argc, char **argv)
{
	const char	*map_path;
	t_ivec2	window_size;

	map_path = argv[1];
	bzero(&g_man, sizeof(t_man));
	if (argc == 1)
		map_path = MAP_DEFAULT;
	else if (argc > 2)
		return (put_error(0, E_TOO_MANY_ARGS, 0, EXIT_FAILURE));
	set_ivec2(&window_size, WINDOW_WIDTH, WINDOW_HEIGHT);
	if (!create_window(&g_man, TITLE, window_size, 16.0 / 9)
		|| !init_frame(&g_man)
		|| !update_image_array(&g_man, "img/common/index.json"))
		return (put_error(&g_man, 0, 0, EXIT_FAILURE));
	add_outline_to_font(get_image(&g_man, "font_default"));
	audio_init(&g_man.audio);
	init_minimap_values(&g_man);
	init_fog(&g_man);
	if (!init_game(&g_man, map_path))
		return (EXIT_FAILURE);
	init_input_handling(&g_man);
	run_game_loop(&g_man);
	deinit(&g_man);
	return (EXIT_SUCCESS);
}

static int	init_game(t_man *man, const char *map_path)
{
	man->curr_map = add_map(man, map_path);
	if (man->curr_map < 0)
		return (put_error(man, 0, 0, 0));
	reset_player(man);
	change_game_state(man, GAME_STATE_PLAY);
	return (1);
}
