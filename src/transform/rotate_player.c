#include "cimmerian.h"

void	rotate_player(double angle)
{
	t_vec2	dir;
	t_vec2	plane;

	dir = g_man.player.dir;
	plane = g_man.player.plane;
	g_man.player.dir.x = dir.x * cos_f(angle) - dir.y * sin_f(angle);
	g_man.player.dir.y = dir.x * sin_f(angle) + dir.y * cos_f(angle);
	g_man.player.plane.x = plane.x * cos_f(angle) - plane.y * sin_f(angle);
	g_man.player.plane.y = plane.x * sin_f(angle) + plane.y * cos_f(angle);
	return ;
}
