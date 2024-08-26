#include "cimmerian.h"

static void move_along_forward_axis(double speed);
static void move_along_lateral_axis(double speed);
static void prevent_out_of_bounds(t_map* m, double radius);
static void adjust_position_on_collision(t_map* m, double radius);
static void rotate(double angle);
static double get_angle_from_dir(t_vec2 dir);

void reset_player_transform(t_map* m)
{
    man.player.pos.x = m->start_pos.x;
    man.player.pos.y = m->start_pos.y;
    man.player.dir.x = 1;
    man.player.dir.y = 0;
    man.player.plane.x = 0;
    man.player.plane.y = f_tan(deg2rad(FOV) / 2.0);
    man.player.height = 0;
    rotate(get_angle_from_dir(m->start_dir));
    return;
}

/*
    Movement speed is in cell per second.
    The radius is the player size.
    Rotation angle is in radian per second.
*/
void update_player_transform(t_map* m)
{
    move_along_forward_axis(2.0 * man.movement_action[2] * man.delta_time);
    move_along_lateral_axis(2.0 * man.movement_action[0] * man.delta_time);
    prevent_out_of_bounds(m, 0.25);
    adjust_position_on_collision(m, 0.25);
    rotate(RAD_45 * man.rotation_action * man.delta_time);
    return;
}

static void move_along_forward_axis(double speed)
{
    man.player.pos.x += man.player.dir.x * speed;
    man.player.pos.y += man.player.dir.y * speed;
    return;
}

static void move_along_lateral_axis(double speed)
{
    man.player.pos.x -= man.player.dir.y * speed;
    man.player.pos.y += man.player.dir.x * speed;
    return;
}

static void prevent_out_of_bounds(t_map* m, double radius)
{
    t_vec2 pos;

    pos = man.player.pos;
    pos.x = f_clamp(pos.x, 1 + radius, m->size.x - 1 - radius);
    pos.y = f_clamp(pos.y, 1 + radius, m->size.y - 1 - radius);
    man.player.pos = pos;
    return;
}

static void adjust_position_on_collision(t_map* m, double radius)
{
    t_vec2 pos;

    pos = man.player.pos;
    if (m->data[(int)(pos.y) * m->size.x + (int)(pos.x + radius)])
        pos.x = f_floor(pos.x + radius) - radius;
    if (m->data[(int)(pos.y) * m->size.x + (int)(pos.x - radius)])
        pos.x = f_ceil(pos.x - radius) + radius;
    if (m->data[(int)(pos.y + radius) * m->size.x + (int)(pos.x)])
        pos.y = f_floor(pos.y + radius) - radius;
    if (m->data[(int)(pos.y - radius) * m->size.x + (int)(pos.x)])
        pos.y = f_ceil(pos.y - radius) + radius;
    man.player.pos = pos;
    return;
}

static void rotate(double angle)
{
    t_vec2 dir;
    t_vec2 plane;

    dir = man.player.dir;
    plane = man.player.plane;
    man.player.dir.x = dir.x * f_cos(angle) - dir.y * f_sin(angle);
    man.player.dir.y = dir.x * f_sin(angle) + dir.y * f_cos(angle);
    man.player.plane.x = plane.x * f_cos(angle) - plane.y * f_sin(angle);
    man.player.plane.y = plane.x * f_sin(angle) + plane.y * f_cos(angle);
    return;
}

double atan2(double y, double x);
static double get_angle_from_dir(t_vec2 dir)
{
    if (dir.x != 0 || dir.y != 0)
        return atan2(dir.y, dir.x);
    return 0;
}
