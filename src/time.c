#include "cimmerian.h"

double delta_time = 0.0f;
double fps_count = 0.0f;
static double last_time = 0.0f;

void update_time_variables(void)
{
    double current_time = glfwGetTime();
    delta_time = current_time - last_time;
    last_time = current_time;
    fps_count = 1/delta_time;
    return;
}
