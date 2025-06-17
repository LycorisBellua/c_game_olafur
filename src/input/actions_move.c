#include "olafur.h"

void	action_move_right(t_man *man, int set)
{
	if (set)
		man->move_action.x += 1;
	else
		man->move_action.x += -1;
	return ;
}

void	action_move_left(t_man *man, int set)
{
	if (set)
		man->move_action.x += -1;
	else
		man->move_action.x += 1;
	return ;
}

void	action_move_forward(t_man *man, int set)
{
	if (set)
		man->move_action.y += 1;
	else
		man->move_action.y += -1;
	return ;
}

void	action_move_backward(t_man *man, int set)
{
	if (set)
		man->move_action.y += -1;
	else
		man->move_action.y += 1;
	return ;
}
