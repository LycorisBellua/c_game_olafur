#include "../headers/camera.h"
#include "../headers/input.h"
#include "../headers/uniforms.h"
#include "../headers/maths.h"

int camera_moves = 0;
float camera_transform[9] = {0};
float camera_speed[3] = { -0.02f, -0.01f, -0.01f };
static float camera_rotation_speed = 0.5f;

/* Called from rendering loop for smooth movement */
void move_camera(void)
{
    /* Translate camera transform */
    /*
    camera_transform[6] += movement_action[0] * camera_speed[0];
    camera_transform[8] += movement_action[2] * camera_speed[2];
    */

    /* Rotate camera transform */
    /*
    float yaw = clamp_euler_angle(camera_transform[4] + rotation_action 
        * camera_rotation_speed);
    camera_transform[4] = yaw;
    */

    /* Update view matrix */
    /*
    compose_transform_matrix(UNIFORM_VIEW_MATRIX->data, 
        camera_transform[0], camera_transform[1], camera_transform[2], 
        camera_transform[3], camera_transform[4], camera_transform[5], 
        camera_transform[6], camera_transform[7], camera_transform[8]);
    UNIFORM_VIEW_MATRIX->activate(UNIFORM_VIEW_MATRIX, 1);
    */
    return;
}

