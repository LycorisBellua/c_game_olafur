#include "cimmerian.h"

double		atan2(double y, double x);
static void	move_along_forward_axis(double speed);
static void	move_along_lateral_axis(double speed);
static void	prevent_out_of_bounds(t_map *m, double radius);
static void	adjust_position_on_collision(t_map *m, double radius);
static void	rotate(double angle);

void	reset_player_transform(t_map *m)
{
	g_man.movement_speed = DEFAULT_MOVE_SPEED;
	g_man.rotation_speed = DEFAULT_ROT_SPEED;
	g_man.player.pos.x = m->start_pos.x;
	g_man.player.pos.y = m->start_pos.y;
	g_man.player.dir.x = 1;
	g_man.player.dir.y = 0;
	g_man.player.plane.x = 0;
	g_man.player.plane.y = f_tan(deg2rad(FOV) / 2.0);
	rotate(get_angle_from_dir(m->start_dir));
	return ;
}

/*
	Movement speed is in cell per second.
	The radius is the player size.
	Rotation angle is in radian per second.
*/
void	update_player_transform(t_map *m)
{
	move_along_forward_axis(g_man.movement_speed * norm(g_man.movement_action.y)
		* g_man.dt);
	move_along_lateral_axis(g_man.movement_speed * norm(g_man.movement_action.x)
		* g_man.dt);
	prevent_out_of_bounds(m, g_man.rotation_speed);
	adjust_position_on_collision(m, g_man.rotation_speed);
	rotate(RAD_45 * norm(g_man.rotation_action) * g_man.dt);
	return ;
}

double	get_angle_from_dir(t_vec2 dir)
{
	if (dir.x != 0 || dir.y != 0)
		return (atan2(dir.y, dir.x));
	return (0);
}

static void	move_along_forward_axis(double speed)
{
	g_man.player.pos.x += g_man.player.dir.x * speed;
	g_man.player.pos.y += g_man.player.dir.y * speed;
	return ;
}

static void	move_along_lateral_axis(double speed)
{
	g_man.player.pos.x -= g_man.player.dir.y * speed;
	g_man.player.pos.y += g_man.player.dir.x * speed;
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
	if (m->cells[(int)(pos.y) * m->size.x + (int)(pos.x + radius)].is_obstacle)
		pos.x = f_floor(pos.x + radius) - radius;
	if (m->cells[(int)(pos.y) * m->size.x + (int)(pos.x - radius)].is_obstacle)
		pos.x = f_ceil(pos.x - radius) + radius;
	if (m->cells[(int)(pos.y + radius) * m->size.x + (int)(pos.x)].is_obstacle)
		pos.y = f_floor(pos.y + radius) - radius;
	if (m->cells[(int)(pos.y - radius) * m->size.x + (int)(pos.x)].is_obstacle)
		pos.y = f_ceil(pos.y - radius) + radius;
	g_man.player.pos = pos;
	return ;
}

static void	rotate(double angle)
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
