#include "cimmerian.h"

void	rotate_player(double angle)
{
	t_vec2	dir;
	t_vec2	plane;

	dir = g_man.player.dir;
	plane = g_man.player.plane;
	g_man.player.dir.x = dir.x * f_cos(angle) - dir.y * f_sin(angle);
	g_man.player.dir.y = dir.x * f_sin(angle) + dir.y * f_cos(angle);
	g_man.player.plane.x = plane.x * f_cos(angle) - plane.y * f_sin(angle);
	g_man.player.plane.y = plane.x * f_sin(angle) + plane.y * f_cos(angle);
	return ;
}
