#include "olafur.h"

void	action_run(t_man *man, int set)
{
	if (set)
	{
		man->move_speed = DEFAULT_MOVE_SPEED * 2;
		man->rotate_speed = DEFAULT_ROTATE_SPEED * 2;
	}
	else
	{
		man->move_speed = DEFAULT_MOVE_SPEED;
		man->rotate_speed = DEFAULT_ROTATE_SPEED;
	}
	return ;
}
