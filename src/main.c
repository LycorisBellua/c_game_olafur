#include "olafur.h"

static void	fetch_default_images(t_man *man);
static int	init_game(t_man *man);

t_man	g_man;

int	main(int argc, char **argv)
{
	t_ivec2	window_size;

	memset(&g_man, 0, sizeof(t_man));
	g_man.vsync = 1;
	if (argc == 1)
		g_man.first_map_path = find_first_map_path(MAP_DIR);
	else if (argc == 2)
		g_man.first_map_path = strdup(argv[1]);
	else
		return (put_error(0, E_TOO_MANY_ARGS, 0, EXIT_FAILURE));
	set_ivec2(&window_size, WINDOW_WIDTH, WINDOW_HEIGHT);
	if (!create_window(&g_man, TITLE, window_size, ASPECT_RATIO)
		|| !init_frame(&g_man)
		|| !update_image_array(&g_man, IMG_JSON_DEFAULT))
		return (put_error(&g_man, 0, 0, EXIT_FAILURE));
	fetch_default_images(&g_man);
	audio_init(&g_man.audio);
	init_minimap_values(&g_man);
	init_fog(&g_man);
	if (!init_game(&g_man))
		return (EXIT_FAILURE);
	init_input_handling(&g_man);
	run_game_loop(&g_man);
	deinit(&g_man);
	return (EXIT_SUCCESS);
}

static void	fetch_default_images(t_man *man)
{
	man->img_font = get_image(man, ID_FONT);
	if (man->img_font)
		add_outline_to_font(man->img_font);
	man->img_cursor = get_image(man, ID_CURSOR);
	man->img_collec = get_image(man, ID_GUI_COLLEC);
	man->gui_scale = man->res.res.y / 360;
	if (man->gui_scale < 1)
		man->gui_scale = 1;
	return ;
}

static int	init_game(t_man *man)
{
	man->curr_map = add_map(man, man->first_map_path);
	if (man->curr_map < 0)
		return (put_error(man, 0, 0, 0));
	reset_player(man);
	change_game_state(man, GAME_STATE_PLAY);
	return (1);
}
