#include "cimmerian.h"

static double get_safe_angle(double angle);
static void update_rotation(void);
static void update_position(t_map* m);

void reset_player_transform(t_map* m)
{
    man.player.pos.x = m->start_pos.x;
    man.player.pos.y = m->start_pos.y;
    man.player.angle = get_safe_angle(m->start_angle);
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

static double get_safe_angle(double angle)
{
    /* The function is to prevent errors with f_tan in the raycasting */

    if (angle <= 0)
        return 0.0001;

    if (double_equality(RAD_90, angle))
        return angle - 0.0001;
    
    if (double_equality(RAD_180, angle))
        return angle - 0.0001;
    
    if (double_equality(RAD_270, angle))
        return angle - 0.0001;
    
    return angle;
}

static void update_rotation(void)
{
    double speed;

    speed = 30.0;
    if (man.rotation_action)
    {
        man.player.angle += man.rotation_action * RAD_1 * speed * man.delta_time;
        man.player.angle = get_safe_angle(clamp_rad(man.player.angle));

        /*
            The values returned by the sine and cosine operations are 
            very small, so multiply them by 5.
        */
        man.player.delta.x = f_cos(man.player.angle) * 5;
        man.player.delta.y = f_sin(man.player.angle) * 5;
    }
    return;
}

static void update_position(t_map* m)
{
    double speed;
    t_vec2 pos;
    t_vec2 size;
    int index;

    speed = 15.0;
    if (man.movement_action[2] + man.movement_action[0] == 0)
        return;

    pos = man.player.pos;
    size.x = man.player.delta.x < 0 ? -20 : 20;
    size.y = man.player.delta.y < 0 ? -20 : 20;

    /* Movement along the forward axis */
    index = (int)(m->size.y - pos.y / MAP_CELL_LEN) * m->size.x 
        + (int)((pos.x + man.movement_action[2] * size.x) / MAP_CELL_LEN);
    if (m->data[index] == 0)
        pos.x += man.movement_action[2] * man.player.delta.x * speed * man.delta_time;

    index = (int)(m->size.y - (pos.y + man.movement_action[2] * size.y) / MAP_CELL_LEN) 
        * m->size.x + (int)(pos.x / MAP_CELL_LEN);
    if (m->data[index] == 0)
        pos.y += man.movement_action[2] * man.player.delta.y * speed * man.delta_time;

    /* Movement along the lateral axis */
    index = (int)(m->size.y - pos.y / MAP_CELL_LEN) * m->size.x 
        + (int)((pos.x + man.movement_action[0] * size.y) / MAP_CELL_LEN);
    if (m->data[index] == 0)
        pos.x += man.movement_action[0] * man.player.delta.y * speed * man.delta_time;

    index = (int)(m->size.y - (pos.y + man.movement_action[0] * -size.y) / MAP_CELL_LEN) 
        * m->size.x + (int)(pos.x / MAP_CELL_LEN);
    if (m->data[index] == 0)
        pos.y += man.movement_action[0] * -man.player.delta.x * speed * man.delta_time;

    man.player.pos = pos;
    return;
}
