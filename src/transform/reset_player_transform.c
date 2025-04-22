#include "cimmerian.h"

/* A FOV of 53 degrees is used to make the walls perfectly cubic. */
void	reset_player_transform(t_man *man)
{
	if (!man->move_speed)
	{
		man->move_speed = DEFAULT_MOVE_SPEED;
		man->rotate_speed = DEFAULT_ROTATE_SPEED;
	}
	man->player.radius = DEFAULT_PLAYER_RADIUS;
	set_vec2(&man->player.pos, man->maps[man->curr_map]->start_pos.x,
		man->maps[man->curr_map]->start_pos.y);
	man->player.prev_pos = man->player.pos;
	set_vec2(&man->player.dir, 1, 0);
	set_vec2(&man->player.plane, 0, tan(deg2rad(53) / 2.0) * man->res.ratio);
	rotate_player(man, get_angle_from_dir(man->maps[man->curr_map]->start_dir));
	man->player.to_collect = SPRITE_AMOUNT_TO_COLLECT;
	return ;
}
