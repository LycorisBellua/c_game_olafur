#include "cimmerian.h"

void	action_speed_walk(t_man *man)
{
	man->move_speed = DEFAULT_MOVE_SPEED;
	man->rotate_speed = DEFAULT_ROTATE_SPEED;
	return ;
}

void	action_speed_run(t_man *man)
{
	man->move_speed = DEFAULT_MOVE_SPEED * 2;
	man->rotate_speed = DEFAULT_ROTATE_SPEED * 2;
	return ;
}
