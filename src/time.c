#include "cimmerian.h"

float delta_time = 0.0f;
float fps_count = 0.0f;
static float last_time = 0.0f;

void update_time_variables(void)
{
    float current_time = glfwGetTime();
    delta_time = current_time - last_time;
    last_time = current_time;
    fps_count = 1/delta_time;
    return;
}
