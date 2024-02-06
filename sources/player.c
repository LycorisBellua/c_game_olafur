#include "../headers/player.h"
#include "../headers/input.h"
#include "../headers/time.h"
#include "../headers/maths.h"

Player player = {0};

static float get_safe_angle(const float angle);

void reset_player_transform(const Map* m)
{
    player.pos.x = m->start_pos.x;
    player.pos.y = m->start_pos.y;
    player.angle = get_safe_angle(m->start_angle);
    player.delta.x = f_cos(player.angle)*5;
    player.delta.y = f_sin(player.angle)*5;
    return;
}

void update_player_transform(void)
{
    const float movement_speed = 15.0f;
    const float rotation_speed = 30.0f;

    player.angle += rotation_action * RAD_1 * rotation_speed * delta_time;
    if (rotation_action)
    {
        player.angle = clamp_radians(player.angle);
        player.angle = get_safe_angle(player.angle);

        /*
            The values returned by the sine and cosine operations are 
            very small, so multiply them by 5.
        */
        player.delta.x = f_cos(player.angle)*5;
        player.delta.y = f_sin(player.angle)*5;
    }

    /* Movement along the forward axis */
    player.pos.x += movement_action[2] * player.delta.x * movement_speed * 
        delta_time;
    player.pos.y += movement_action[2] * player.delta.y * movement_speed * 
        delta_time;

    /* Movement along the lateral axis */
    player.pos.x += movement_action[0] * player.delta.y * movement_speed * 
        delta_time;
    player.pos.y += movement_action[0] * -player.delta.x * movement_speed * 
        delta_time;
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

