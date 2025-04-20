#include "cimmerian.h"

void	action_rotate_incr(t_man *man)
{
	man->rotate_action += 1;
	return ;
}

void	action_rotate_decr(t_man *man)
{
	man->rotate_action += -1;
	return ;
}
