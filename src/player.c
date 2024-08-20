#include "cimmerian.h"

static void update_rotation(void);
static void update_position(t_map* m);

void reset_player_transform(t_map* m)
{
    man.player.pos.x = m->start_pos.x;
    man.player.pos.y = m->start_pos.y;
    man.player.dir.x = 1;
    man.player.dir.y = 0;
    man.player.plane.x = 0;
    man.player.plane.y = f_tan(deg2rad(FOV) / 2.0);

    man.player.angle = m->start_angle;
    man.player.delta.x = f_cos(man.player.angle) * 5;
    man.player.delta.y = f_sin(man.player.angle) * 5;
    return;
}

void update_player_transform(t_map* m)
{
    update_rotation();
    update_position(m);
    return;
}

/*
    The values returned by the sine and cosine operations are 
    very small, so multiply them by 5.
*/
static void update_rotation(void)
{
    double speed;

    if (!man.rotation_action)
        return;
    speed = 30.0;
    man.player.angle += man.rotation_action * RAD_1 * speed * man.delta_time;
    man.player.angle = clamp_rad(man.player.angle);
    man.player.delta.x = f_cos(man.player.angle) * 5;
    man.player.delta.y = f_sin(man.player.angle) * 5;
    return;
}

static void update_position(t_map* m)
{
    double speed;
    t_vec2 pos;
    t_vec2 offset;
    t_ivec2 index;
    t_ivec2 add_offset;
    t_ivec2 sub_offset;

    if (man.movement_action[2] + man.movement_action[0] == 0)
        return;
    speed = 15.0;
    pos = man.player.pos;

    offset.x = man.player.delta.x < 0 ? -20 : 20;
    offset.y = man.player.delta.y < 0 ? -20 : 20;
    index.x = pos.x / MAP_CELL_LEN;
    index.y = pos.y / MAP_CELL_LEN;
    add_offset.x = (pos.x + offset.x) / MAP_CELL_LEN;
    add_offset.y = (pos.y + offset.y) / MAP_CELL_LEN;
    sub_offset.x = (pos.x - offset.x) / MAP_CELL_LEN;
    sub_offset.y = (pos.y - offset.y) / MAP_CELL_LEN;

    /* Movement along the forward axis */
    /* TODO: Refactor (ONLY "ADD" or "SUB" CHANGES) */
    if (man.movement_action[2] > 0)
    {
        if (m->data[index.y * m->size.x + add_offset.x] == 0)
            pos.x += man.movement_action[2] * man.player.delta.x * speed * man.delta_time;
        if (m->data[add_offset.y * m->size.x + index.x] == 0)
            pos.y += man.movement_action[2] * man.player.delta.y * speed * man.delta_time;
    }
    else if (man.movement_action[2] < 0)
    {
        if (m->data[index.y * m->size.x + sub_offset.x] == 0)
            pos.x += man.movement_action[2] * man.player.delta.x * speed * man.delta_time;
        if (m->data[sub_offset.y * m->size.x + index.x] == 0)
            pos.y += man.movement_action[2] * man.player.delta.y * speed * man.delta_time;
    }

    /* Movement along the lateral axis */
    /* TODO: Collision on lateral movement */
    if (man.movement_action[0] > 0)
    {
        /*
        pos.x += man.movement_action[0] * -man.player.delta.y * speed * man.delta_time;
        pos.y += man.movement_action[0] * man.player.delta.x * speed * man.delta_time;
        */
    }
    else if (man.movement_action[0] < 0)
    {
        /*
        pos.x += man.movement_action[0] * -man.player.delta.y * speed * man.delta_time;
        pos.y += man.movement_action[0] * man.player.delta.x * speed * man.delta_time;
        */
    }

    man.player.pos = pos;
    return;
}
