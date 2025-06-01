#include "cimmerian.h"

void	reset_player(t_man *man)
{
	if (!man->move_speed)
	{
		man->move_speed = DEFAULT_MOVE_SPEED;
		man->rotate_speed = DEFAULT_ROTATE_SPEED;
	}
	man->player.radius = DEFAULT_PLAYER_RADIUS;
	man->player.collected = 0;
	man->player.to_collect = SPRITE_AMOUNT_TO_COLLECT;
	reset_player_transform(man);
	return ;
}

void	reset_player_transform(t_man *man)
{
	set_player_transform(man, man->maps[man->curr_map]->start_pos,
		man->maps[man->curr_map]->start_dir);
	return ;
}

/* A FOV of 53 degrees is used to make the walls perfectly cubic. */
void	set_player_transform(t_man *man, t_vec2 new_pos, t_vec2 new_dir)
{
	set_vec2(&man->player.pos, new_pos.x, new_pos.y);
	man->player.prev_pos = man->player.pos;
	set_vec2(&man->player.dir, 1, 0);
	set_vec2(&man->player.plane, 0, tan(deg2rad(53) / 2.0) * man->res.ratio);
	rotate_player(man, get_angle_from_dir(new_dir));
	audio_switch_music(man);
	return ;
}
