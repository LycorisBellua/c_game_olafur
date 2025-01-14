#include "cimmerian.h"

static void	move_in_local_dir(double forward_speed, double lateral_speed);
static void	prevent_out_of_bounds(t_map *m, double radius);
static void	adjust_position_on_collision(t_map *m, double radius);

/*
	Movement speed is in cell per second.
	The radius is the player size.
	Rotation angle is in radian per second.
*/

void	update_player_transform(t_map *m)
{
	double	forward_speed;
	double	lateral_speed;

	forward_speed = g_man.move_speed * norm(g_man.move_action.y) * g_man.dt;
	lateral_speed = g_man.move_speed * norm(g_man.move_action.x) * g_man.dt;
	move_in_local_dir(forward_speed, lateral_speed);
	prevent_out_of_bounds(m, g_man.rotate_speed);
	adjust_position_on_collision(m, g_man.rotate_speed);
	rotate_player(RAD_45 * norm(g_man.rotate_action) * g_man.dt);
	echolocation(forward_speed || lateral_speed);
	return ;
}

static void	move_in_local_dir(double forward_speed, double lateral_speed)
{
	g_man.player.pos.x += g_man.player.dir.x * forward_speed;
	g_man.player.pos.y += g_man.player.dir.y * forward_speed;
	g_man.player.pos.x -= g_man.player.dir.y * lateral_speed;
	g_man.player.pos.y += g_man.player.dir.x * lateral_speed;
	return ;
}

static void	prevent_out_of_bounds(t_map *m, double radius)
{
	t_vec2	pos;

	pos = g_man.player.pos;
	pos.x = f_clamp(pos.x, 1 + radius, m->size.x - 1 - radius);
	pos.y = f_clamp(pos.y, 1 + radius, m->size.y - 1 - radius);
	g_man.player.pos = pos;
	return ;
}

static void	adjust_position_on_collision(t_map *m, double radius)
{
	t_vec2	pos;

	pos = g_man.player.pos;
	if (m->cells[(int)pos.y * m->size.x + (int)(pos.x + radius)].is_obstacle)
		pos.x = f_floor(pos.x + radius) - radius;
	if (m->cells[(int)pos.y * m->size.x + (int)(pos.x - radius)].is_obstacle)
		pos.x = f_ceil(pos.x - radius) + radius;
	if (m->cells[(int)(pos.y + radius) *m->size.x + (int)pos.x].is_obstacle)
		pos.y = f_floor(pos.y + radius) - radius;
	if (m->cells[(int)(pos.y - radius) *m->size.x + (int)pos.x].is_obstacle)
		pos.y = f_ceil(pos.y - radius) + radius;
	g_man.player.pos = pos;
	return ;
}
