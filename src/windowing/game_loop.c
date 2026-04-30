#include "olafur.h"

void	run_game_loop(t_man *man)
{
	while (!glfwWindowShouldClose(man->window))
		game_loop(man);
	return ;
}
