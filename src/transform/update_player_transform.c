#include "cimmerian.h"

static void	move_in_local_dir(t_man *man, double forward_speed,
				double lateral_speed);
static void	prevent_out_of_bounds(t_man *man);
static void	adjust_position_on_collision(t_man *man);

/*
	Movement speed is in cell per second.
	The radius is the player size.
	Rotation angle is in radian per second.
*/

void	update_player_transform(t_man *man)
{
	double	forward_speed;
	double	lateral_speed;

	forward_speed = man->move_speed * norm(man->move_action.y) * man->dt;
	lateral_speed = man->move_speed * norm(man->move_action.x) * man->dt;
	move_in_local_dir(man, forward_speed, lateral_speed);
	prevent_out_of_bounds(man);
	adjust_position_on_collision(man);
	rotate_player(man, RAD_45 * norm(man->rotate_action) * man->dt);
	echolocation(man, forward_speed || lateral_speed);
	return ;
}

static void	move_in_local_dir(t_man *man, double forward_speed,
	double lateral_speed)
{
	man->player.pos.x += man->player.dir.x * forward_speed;
	man->player.pos.y += man->player.dir.y * forward_speed;
	man->player.pos.x -= man->player.dir.y * lateral_speed;
	man->player.pos.y += man->player.dir.x * lateral_speed;
	return ;
}

static void	prevent_out_of_bounds(t_man *man)
{
	double	radius;
	t_vec2	pos;

	radius = man->player.radius;
	pos = man->player.pos;
	pos.x = clamp_f(pos.x, 1 + radius, man->map->size.x - 1 - radius);
	pos.y = clamp_f(pos.y, 1 + radius, man->map->size.y - 1 - radius);
	man->player.pos = pos;
	return ;
}

static void	adjust_position_on_collision(t_man *man)
{
	double	radius;
	t_map	*m;
	t_vec2	pos;

	radius = man->player.radius;
	m = man->map;
	pos = man->player.pos;
	if (m->cells[(int)pos.y * m->size.x + (int)(pos.x + radius)].is_obstacle)
		pos.x = floor_f(pos.x + radius) - radius;
	if (m->cells[(int)pos.y * m->size.x + (int)(pos.x - radius)].is_obstacle)
		pos.x = ceil_f(pos.x - radius) + radius;
	if (m->cells[(int)(pos.y + radius) *m->size.x + (int)pos.x].is_obstacle)
		pos.y = floor_f(pos.y + radius) - radius;
	if (m->cells[(int)(pos.y - radius) *m->size.x + (int)pos.x].is_obstacle)
		pos.y = ceil_f(pos.y - radius) + radius;
	man->player.pos = pos;
	return ;
}
