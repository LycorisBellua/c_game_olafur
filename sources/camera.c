#include "../headers/camera.h"
#include "../headers/input.h"
#include "../headers/uniforms.h"
#include "../headers/maths.h"

int camera_moves = 0;
float camera_transform[9] = {0};
float camera_speed[3] = { -0.01f, -0.01f, -0.01f };
static float camera_rotation_speed = 0.1f;

static float clamp_euler_angle(float angle);

/* Called from rendering loop for smooth movement */
void move_camera(void)
{
    float yaw;

    /* Updated in set_active_interface() */
    if (!camera_moves)
        return;

    /* Translate camera transform */
    camera_transform[6] += movement_action[0] * camera_speed[0];
    camera_transform[8] += movement_action[2] * camera_speed[2];

    /* Rotate camera transform */
    yaw = clamp_euler_angle(camera_transform[4] + rotation_action 
        * camera_rotation_speed);
    camera_transform[4] = yaw;

    /* Update view matrix */
    compose_transform_matrix(UNIFORM_VIEW_MATRIX->data, 
        camera_transform[0], camera_transform[1], camera_transform[2], 
        camera_transform[3], camera_transform[4], camera_transform[5], 
        camera_transform[6], camera_transform[7], camera_transform[8]);
    UNIFORM_VIEW_MATRIX->activate(UNIFORM_VIEW_MATRIX, 1);
    return;
}

static float clamp_euler_angle(float angle)
{
    if (angle < -360.0f)
        angle += 360.0f;
    else if (angle > 360.0f)
        angle -= 360.0f;
    return angle;
}

