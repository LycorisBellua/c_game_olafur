#include "cimmerian.h"

void	action_move_x_incr(t_man *man)
{
	man->move_action.x += 1;
	return ;
}

void	action_move_x_decr(t_man *man)
{
	man->move_action.x += -1;
	return ;
}

void	action_move_y_incr(t_man *man)
{
	man->move_action.y += 1;
	return ;
}

void	action_move_y_decr(t_man *man)
{
	man->move_action.y += -1;
	return ;
}
