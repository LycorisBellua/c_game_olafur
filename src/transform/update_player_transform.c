#include "olafur.h"

static void	move_in_local_dir(t_man *man, float forward_speed,
				float lateral_speed);
static void	adjust_position_on_wall_collision(t_man *man);
static void	adjust_position_on_sprite_collision_within_walls(t_man *man);

/*
	Movement speed is in cell per second.
	The radius is the player size.
	Rotation angle is in radian per second.
	The frame time is capped, so that after the game froze, the player isn't
	moved so far in one go that they would go through a wall.
*/

void	update_player_transform(t_man *man)
{
	float	forward_speed;
	float	lateral_speed;
	float	dt;

	dt = fminf(man->dt, MAX_MOVE_DT);
	forward_speed = man->move_speed * norm(man->move_action.y) * dt;
	lateral_speed = man->move_speed * norm(man->move_action.x) * dt;
	move_in_local_dir(man, forward_speed, lateral_speed);
	prevent_out_of_bounds(man);
	adjust_position_on_wall_collision(man);
	adjust_position_on_sprite_collision_within_walls(man);
	rotate_player(man, man->rotate_speed * RAD_45 * norm(man->rotate_action)
		* dt);
	if (man->r_click_action)
		rotate_player(man, man->drag_rotate_speed * dt);
	audio_listener_transform(&man->player);
	return ;
}

void	prevent_out_of_bounds(t_man *man)
{
	t_vec2	pos;

	pos = man->player.pos;
	pos.x = fclamp(pos.x, 0.01f, man->maps[man->curr_map]->size.x - 0.01f);
	pos.y = fclamp(pos.y, 0.01f, man->maps[man->curr_map]->size.y - 0.01f);
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

/*
	Being pushed out of a sprite can push the player into a wall. The walls are
	then resolved again, against the push, so that the player is never shown
	inside a wall: squeezed between a sprite and a wall, the player overlaps
	the sprite a little instead.
*/
static void	adjust_position_on_sprite_collision_within_walls(t_man *man)
{
	t_map	*m;
	t_vec2	before;
	t_vec2	push;

	m = man->maps[man->curr_map];
	before = man->player.pos;
	adjust_position_on_sprite_collision(man, m);
	push.x = man->player.pos.x - before.x;
	push.y = man->player.pos.y - before.y;
	if (!push.x && !push.y)
		return ;
	if (!push_back_on_collision(man, m, man->player.pos, push))
		unstuck_from_wall(man, m);
	return ;
}
