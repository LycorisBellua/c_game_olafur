#include "cimmerian.h"

static void move_along_forward_axis(double speed);
static void move_along_lateral_axis(double speed);
static void adjust_position_on_collision(double radius);
static void rotate(double angle);
static t_vec2 get_cardinal_dir(char c);
static double get_angle_from_dir(t_vec2 dir);

void reset_player_transform(t_map* m)
{
    man.player.pos.x = m->start_pos.x;
    man.player.pos.y = m->start_pos.y;
    man.player.dir.x = 1;
    man.player.dir.y = 0;
    man.player.plane.x = 0;
    man.player.plane.y = f_tan(deg2rad(FOV) / 2.0);
    rotate(get_angle_from_dir(get_cardinal_dir('N')));
    return;
}

/*
    Movement speed is in cell per second.
    The radius is the player size.
    Rotation angle is in radian per second.
*/
void update_player_transform(void)
{
    move_along_forward_axis(2.0 * man.movement_action[2] * man.delta_time);
    move_along_lateral_axis(2.0 * man.movement_action[0] * man.delta_time);
    adjust_position_on_collision(0.25);
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

static void adjust_position_on_collision(double radius)
{
    t_vec2 pos;

    pos = man.player.pos;
    if (map[(int)(pos.y)][(int)(pos.x + radius)])
        pos.x = f_floor(pos.x + radius) - radius;
    if (map[(int)(pos.y)][(int)(pos.x - radius)])
        pos.x = f_ceil(pos.x - radius) + radius;
    if (map[(int)(pos.y + radius)][(int)(pos.x)])
        pos.y = f_floor(pos.y + radius) - radius;
    if (map[(int)(pos.y - radius)][(int)(pos.x)])
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

static t_vec2 get_cardinal_dir(char c)
{
    t_vec2 dir;

    dir.x = 0;
    dir.y = 0;
    if (c >= 'a' && c <= 'z')
        c -= 'a' - 'A';
    if (c == 'N')
        dir.y = -1;
    else if (c == 'S')
        dir.y = 1;
    else if (c == 'W')
        dir.x = -1;
    else if (c == 'E')
        dir.x = 1;
    return dir;
}

static double get_angle_from_dir(t_vec2 dir)
{
    if (dir.x != 0 || dir.y != 0)
        return atan2(dir.y, dir.x);
    return 0;
}
