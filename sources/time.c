#include "../headers/time.h"

static float last_time = 0.0f;

float get_delta_time(void)
{
    float current_time = glfwGetTime();
    float delta_time = current_time - last_time;
    last_time = current_time;
    return delta_time;
}

