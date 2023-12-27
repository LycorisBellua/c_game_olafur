#include "../headers/transform.h"
#include "../headers/input.h"
#include "../headers/uniforms.h"
#include "../headers/maths.h"

float player_speed[3] = { -0.01f, -0.01f, 0.007f };

void move_player(void)
{
    float yaw;

    *((float*)UNIFORM_POS_OFFSET->data + 0) += movement_action[0] 
        * player_speed[0];
    *((float*)UNIFORM_POS_OFFSET->data + 2) += movement_action[2] 
        * player_speed[2];
    UNIFORM_POS_OFFSET->activate(UNIFORM_POS_OFFSET, 1);

    yaw = *((float*)UNIFORM_EULER_ANGLES->data + 1) + rotation_action 
        * player_speed[1];
    if (yaw < -360.0f)
        yaw += 360.0f;
    else if (yaw > 360.0f)
        yaw -= 360.0f;
    *((float*)UNIFORM_EULER_ANGLES->data + 1) = yaw;
    UNIFORM_EULER_ANGLES->activate(UNIFORM_EULER_ANGLES, 1);

    return;
}

/* called from set_active_interface() */
void straighten_pitch(void)
{
    *((float*)UNIFORM_EULER_ANGLES->data + 0) = 0.0f;
    UNIFORM_EULER_ANGLES->activate(UNIFORM_EULER_ANGLES, 1);
    return;
}

/* called from set_active_interface() */
void set_pitch_back(void)
{
    *((float*)UNIFORM_EULER_ANGLES->data + 0) = deg2rad(pitch);
    UNIFORM_EULER_ANGLES->activate(UNIFORM_EULER_ANGLES, 1);
    return;
}

