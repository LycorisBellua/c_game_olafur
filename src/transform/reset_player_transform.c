#include "cimmerian.h"

void	reset_player_transform(t_map *m)
{
	g_man.move_speed = DEFAULT_MOVE_SPEED;
	g_man.rotate_speed = DEFAULT_ROTATE_SPEED;
	g_man.player.pos.x = m->start_pos.x;
	g_man.player.pos.y = m->start_pos.y;
	g_man.player.dir.x = 1;
	g_man.player.dir.y = 0;
	g_man.player.plane.x = 0;
	g_man.player.plane.y = f_tan(deg2rad(FOV) / 2.0);
	rotate_player(get_angle_from_dir(m->start_dir.x, m->start_dir.y));
	return ;
}
