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

    pos = man.player.pos;
    dir = man.player.dir;
    /* Move along the forward axis */
    speed = man.movement_action[2] * 2.0 * man.delta_time;
    radius = man.movement_action[2] * 0.2;
    if (!map[(int)(pos.y)][(int)(pos.x + (dir.x * speed) + (dir.x * radius))])
        pos.x += dir.x * speed;
    if (!map[(int)(pos.y + (dir.y * speed) + (dir.y * radius))][(int)pos.x])
        pos.y += dir.y * speed;
    /* Move along the lateral axis */
    speed = man.movement_action[0] * 2.0 * man.delta_time;
    radius = man.movement_action[0] * 0.2;
    if (!map[(int)(pos.y)][(int)(pos.x + (-dir.y * speed) + (-dir.y * radius))])
        pos.x += -dir.y * speed;
    if (!map[(int)(pos.y + (dir.x * speed) + (dir.x * radius))][(int)pos.x])
        pos.y += dir.x * speed;
    man.player.pos = pos;
    return;
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
