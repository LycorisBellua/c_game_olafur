#include "cimmerian.h"

void	action_turn_right(t_man *man, int set)
{
	if (set)
		man->rotate_action += 1;
	else
		man->rotate_action += -1;
	return ;
}

void	action_turn_left(t_man *man, int set)
{
	if (set)
		man->rotate_action += -1;
	else
		man->rotate_action += 1;
	return ;
}
