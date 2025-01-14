#include "cimmerian.h"

void	echolocation(int has_player_moved)
{
	static double	dof_elapsed_time;

	if (has_player_moved)
	{
		dof_elapsed_time += g_man.dt;
		update_dof(g_man.map, dof_elapsed_time / 5);
	}
	else
	{
		dof_elapsed_time = 0;
		update_dof(g_man.map, -2 * g_man.dt);
	}
	return ;
}
