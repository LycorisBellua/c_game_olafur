#include "cimmerian.h"

void	rotate_player(t_man *man, double angle)
{
	t_vec2	dir;
	t_vec2	plane;

	dir = man->player.dir;
	plane = man->player.plane;
	man->player.dir.x = dir.x * cos_f(angle) - dir.y * sin_f(angle);
	man->player.dir.y = dir.x * sin_f(angle) + dir.y * cos_f(angle);
	man->player.plane.x = plane.x * cos_f(angle) - plane.y * sin_f(angle);
	man->player.plane.y = plane.x * sin_f(angle) + plane.y * cos_f(angle);
	return ;
}
