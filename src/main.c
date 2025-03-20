#include "cimmerian.h"

t_man	g_man;

int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		dprintf(2, "Error: Incorrect amount of arguments - Need 1 for the map "
			"file path\n");
		return (EXIT_FAILURE);
	}
	bzero(&g_man, sizeof(t_man));
	if (!create_window(&g_man, TITLE, RES_WIDTH, RES_HEIGHT))
		return (EXIT_FAILURE);
	if (!init_frames(&g_man) || !set_image_array(&g_man, "img/index.json")
		|| !create_map(&g_man, argv[1]))
	{
		deinit(&g_man);
		return (EXIT_FAILURE);
	}
	add_outline_to_font(&g_man.images[1]);
	reset_player_transform(&g_man);
	init_input_handling(&g_man);
	run_game_loop(&g_man);
	return (EXIT_SUCCESS);
}
