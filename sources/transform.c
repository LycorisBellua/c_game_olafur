#include "../headers/transform.h"
#include "../headers/input.h"
#include "../headers/uniforms.h"
#include "../headers/maths.h"

int zero_player_transform = 1;
float player_speed[3] = { -0.01f, -0.01f, 0.007f };

/*
    DEFAULT
    - Euler angles X is deg2rad(pitch = 15.0f)
    - Pos offset Y is -1.0f
*/

/*
static float clamp_euler_angle(float angle);
*/

/* Called from set_active_interface() */
void deactivate_player_transform(void)
{
    /*
    if (zero_player_transform)
    {
        UNIFORM_POS_OFFSET->activate(UNIFORM_POS_OFFSET, 0);
        UNIFORM_EULER_ANGLES->activate(UNIFORM_EULER_ANGLES, 0);
    }
    else
    {
        UNIFORM_POS_OFFSET->activate(UNIFORM_POS_OFFSET, 1);
        UNIFORM_EULER_ANGLES->activate(UNIFORM_EULER_ANGLES, 1);
    }
    */
    return;
}

/* Called from rendering loop for smooth movement */
void move_player(void)
{
    if (zero_player_transform)
        return;

    /*
    *((float*)UNIFORM_POS_OFFSET->data + 0) += movement_action[0] 
        * player_speed[0];
    *((float*)UNIFORM_POS_OFFSET->data + 2) += movement_action[2] 
        * player_speed[2];
    UNIFORM_POS_OFFSET->activate(UNIFORM_POS_OFFSET, 1);

    *((float*)UNIFORM_EULER_ANGLES->data + 1) = clamp_euler_angle
    (
        *((float*)UNIFORM_EULER_ANGLES->data + 1) + 
        rotation_action * 
        player_speed[1]
    );
    UNIFORM_EULER_ANGLES->activate(UNIFORM_EULER_ANGLES, 1);
    */

    return;
}

/*
static float clamp_euler_angle(float angle)
{
    if (angle < -360.0f)
        angle += 360.0f;
    else if (angle > 360.0f)
        angle -= 360.0f;
    return angle;
}
*/

