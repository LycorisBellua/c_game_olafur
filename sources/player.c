#include "../headers/player.h"
#include "../headers/input.h"
#include "../headers/time.h"
#include "../headers/maths.h"

Player player = {0};

static float get_safe_angle(const float angle);
static void update_rotation(void);
static void update_position(const Map* m);

void reset_player_transform(const Map* m)
{
    player.pos.x = m->start_pos.x;
    player.pos.y = m->start_pos.y;
    player.angle = get_safe_angle(m->start_angle);
    player.delta.x = f_cos(player.angle)*5;
    player.delta.y = f_sin(player.angle)*5;
    return;
}

void update_player_transform(const Map* m)
{
    update_rotation();
    update_position(m);
    return;
}

static float get_safe_angle(const float angle)
{
    /* The function is to prevent errors with f_tan in the raycasting */

    if (angle <= 0)
        return 0.0001f;

    if (float_equality(RAD_90, angle))
        return angle - 0.0001f;
    
    if (float_equality(RAD_180, angle))
        return angle - 0.0001f;
    
    if (float_equality(RAD_270, angle))
        return angle - 0.0001f;
    
    return angle;
}

static void update_rotation(void)
{
    const float speed = 30.0f;

    if (rotation_action)
    {
        player.angle += rotation_action * RAD_1 * speed * delta_time;
        player.angle = get_safe_angle(clamp_radians(player.angle));

        /*
            The values returned by the sine and cosine operations are 
            very small, so multiply them by 5.
        */
        player.delta.x = f_cos(player.angle)*5;
        player.delta.y = f_sin(player.angle)*5;
    }
    return;
}

static void update_position(const Map* m)
{
    const float speed = 15.0f;
    VectorF pos, size;
    int index;

    if (movement_action[2] + movement_action[0] == 0)
        return;

    pos = player.pos;
    size.x = player.delta.x < 0 ? -20 : 20;
    size.y = player.delta.y < 0 ? -20 : 20;

    /* Movement along the forward axis */
    index = (int)(m->height - pos.y/MAP_CELL_LEN)*m->width 
            + (int)((pos.x+movement_action[2]*size.x)/MAP_CELL_LEN);
    if (m->data[index] == 0)
        pos.x += movement_action[2] * player.delta.x * speed * delta_time;

    index = (int)(m->height - (pos.y+movement_action[2]*size.y)/MAP_CELL_LEN) 
            * m->width + (int)(pos.x/MAP_CELL_LEN);
    if (m->data[index] == 0)
        pos.y += movement_action[2] * player.delta.y * speed * delta_time;

    /* Movement along the lateral axis */
    index = (int)(m->height - pos.y/MAP_CELL_LEN)*m->width 
            + (int)((pos.x + movement_action[0]*size.y)/MAP_CELL_LEN);
    if (m->data[index] == 0)
        pos.x += movement_action[0] * player.delta.y * speed * delta_time;

    index = (int)(m->height - (pos.y+movement_action[0]*-size.x)/MAP_CELL_LEN) 
            * m->width + (int)(pos.x/MAP_CELL_LEN);
    if (m->data[index] == 0)
        pos.y += movement_action[0] * -player.delta.x * speed * delta_time;

    player.pos = pos;
    return;
}

