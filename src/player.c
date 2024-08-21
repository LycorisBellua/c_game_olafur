#include "cimmerian.h"

static void update_position(void);
static void update_rotation(void);

void reset_player_transform(t_map* m)
{
    man.player.pos.x = m->start_pos.x;
    man.player.pos.y = m->start_pos.y;
    man.player.dir.x = 1;
    man.player.dir.y = 0;
    man.player.plane.x = 0;
    man.player.plane.y = f_tan(deg2rad(FOV) / 2.0);
    return;
}

void update_player_transform(void)
{
    update_position();
    update_rotation();
    return;
}

/* Speed is in cell per second */
static void update_position(void)
{
    double speed;
    double radius;
    t_vec2 pos;
    t_vec2 dir;

    speed = 2.0 * man.delta_time;
    radius = 0.25;
    pos = man.player.pos;
    dir = man.player.dir;
    /* Move along the forward axis */
    pos.x += man.movement_action[2] * dir.x * speed;
    pos.y += man.movement_action[2] * dir.y * speed;
    /* Move along the lateral axis */
    pos.x += man.movement_action[0] * -dir.y * speed;
    pos.y += man.movement_action[0] * dir.x * speed;
    /* Adjust position on collision */
    if (map[(int)(pos.y)][(int)(pos.x + radius)])
        pos.x = f_floor(pos.x + radius) - radius;
    if (map[(int)(pos.y)][(int)(pos.x - radius)])
        pos.x = f_ceil(pos.x - radius) + radius;
    if (map[(int)(pos.y + radius)][(int)(pos.x)])
        pos.y = f_floor(pos.y + radius) - radius;
    if (map[(int)(pos.y - radius)][(int)(pos.x)])
        pos.y = f_ceil(pos.y - radius) + radius;
    man.player.pos = pos;
}

/* Speed is in radian per second */
static void update_rotation(void)
{
    double speed;
    t_vec2 dir;
    t_vec2 plane;

    dir = man.player.dir;
    plane = man.player.plane;
    speed = man.rotation_action * RAD_45 * man.delta_time;
    man.player.dir.x = dir.x * f_cos(speed) - dir.y * f_sin(speed);
    man.player.dir.y = dir.x * f_sin(speed) + dir.y * f_cos(speed);
    man.player.plane.x = plane.x * f_cos(speed) - plane.y * f_sin(speed);
    man.player.plane.y = plane.x * f_sin(speed) + plane.y * f_cos(speed);
    return;
}
