#include "cimmerian.h"

void	reset_player_transform(t_man *man)
{
	man->move_speed = DEFAULT_MOVE_SPEED;
	man->rotate_speed = DEFAULT_ROTATE_SPEED;
	man->player.radius = DEFAULT_PLAYER_RADIUS;
	man->player.pos.x = man->map->start_pos.x;
	man->player.pos.y = man->map->start_pos.y;
	man->player.dir.x = 1;
	man->player.dir.y = 0;
	man->player.plane.x = 0;
	man->player.plane.y = tan_f(deg2rad(FOV) / 2.0);
	rotate_player(man,
		get_angle_from_dir(man->map->start_dir.x, man->map->start_dir.y));
	return ;
}
