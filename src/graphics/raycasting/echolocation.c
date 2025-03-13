#include "cimmerian.h"

void	echolocation(t_man *man, int has_player_moved)
{
	static double	dof_elapsed_time;

	if (has_player_moved)
	{
		dof_elapsed_time += man->dt;
		update_dof(man->map, dof_elapsed_time / 5);
	}
	else
	{
		dof_elapsed_time = 0;
		update_dof(man->map, -2 * man->dt);
	}
	return ;
}
