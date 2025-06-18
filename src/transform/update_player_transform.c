#include "olafur.h"

static void	move_in_local_dir(t_man *man, float forward_speed,
				float lateral_speed);
static void	adjust_position_on_wall_collision(t_man *man);

/*
	Movement speed is in cell per second.
	The radius is the player size.
	Rotation angle is in radian per second.
*/

void	update_player_transform(t_man *man)
{
	float	forward_speed;
	float	lateral_speed;

	forward_speed = man->move_speed * norm(man->move_action.y) * man->dt;
	lateral_speed = man->move_speed * norm(man->move_action.x) * man->dt;
	move_in_local_dir(man, forward_speed, lateral_speed);
	prevent_out_of_bounds(man);
	adjust_position_on_wall_collision(man);
	adjust_position_on_sprite_collision(man, man->maps[man->curr_map]);
	prevent_out_of_bounds(man);
	rotate_player(man, man->rotate_speed * RAD_45 * norm(man->rotate_action)
		* man->dt);
	if (man->echolocation)
		echolocation(man, forward_speed || lateral_speed);
	audio_listener_transform(&man->player);
	return ;
}

void	prevent_out_of_bounds(t_man *man)
{
	t_vec2	pos;

	pos = man->player.pos;
	pos.x = fclamp(pos.x, 0.01, man->maps[man->curr_map]->size.x - 0.01);
	pos.y = fclamp(pos.y, 0.01, man->maps[man->curr_map]->size.y - 0.01);
	if (pos.x != man->player.pos.x || pos.y != man->player.pos.y)
		man->player.prev_pos = pos;
	man->player.pos = pos;
	return ;
}

static void	move_in_local_dir(t_man *man, float forward_speed,
	float lateral_speed)
{
	man->player.prev_pos = man->player.pos;
	man->player.pos.x += man->player.dir.x * forward_speed;
	man->player.pos.y += man->player.dir.y * forward_speed;
	man->player.pos.x -= man->player.dir.y * lateral_speed;
	man->player.pos.y += man->player.dir.x * lateral_speed;
	return ;
}

static void	adjust_position_on_wall_collision(t_man *man)
{
	t_vec2	pos;
	t_vec2	delta;
	t_map	*m;

	pos = man->player.pos;
	delta.x = pos.x - man->player.prev_pos.x;
	delta.y = pos.y - man->player.prev_pos.y;
	m = man->maps[man->curr_map];
	cross_goal_if_unlocked(man, m);
	disable_collision_with_dst_portal_if_within(man, m, pos);
	if (!push_back_on_collision(man, m, pos, delta))
		unstuck_from_wall(man, m);
	return ;
}
