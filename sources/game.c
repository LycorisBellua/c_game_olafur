#include "../headers/game.h"
#include "../headers/textures.h"
#include "../headers/input.h"
#include "../headers/time.h"
#include "../headers/draw_test.h"

float global_x, global_y;

void draw_game(void)
{
    draw_test_rectangles(TEX_MAIN);
    draw_test_circles(TEX_MAIN);
    draw_test_lines(TEX_MAIN);
    draw_test_corners(TEX_MAIN);
    draw_test_center(TEX_MAIN);
    draw_test_gradient_line(TEX_MAIN);
    return;
}

void reset_global_coordinates(void)
{
    global_x = global_y = 0;
    return;
}

void update_global_coordinates(void)
{
    const float speed = 50.0f;
    global_x += movement_action[0] * speed * delta_time;
    global_y += movement_action[2] * speed * delta_time;
    return;
}

