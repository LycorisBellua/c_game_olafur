#include "cimmerian.h"

static t_color get_lerp_c(t_color c1, t_color c2, int steps, int total_steps);

void draw_line(t_frame* f, t_vert v1, t_vert v2)
{
    int steps;
    int total_steps;
    t_ivec2 dir;
    t_vec2 coord;
    t_vec2 increment;

    dir = get_direction(v1.coord, v2.coord);
    steps = max(abs(dir.x), abs(dir.y));
    coord.x = v1.coord.x;
    coord.y = v1.coord.y;
    increment.x = dir.x / (double)steps;
    increment.y = dir.y / (double)steps;
    ++steps;
    total_steps = steps;
    while (steps-- > 0)
    {
        draw_point(f, get_lerp_c(v1.color, v2.color, steps, total_steps),
            coord.x, coord.y);
        coord.x += increment.x;
        coord.y += increment.y;
    }
    return;
}

static t_color get_lerp_c(t_color c1, t_color c2, int steps, int total_steps)
{
    t_color color;
    double factor;

    if (total_steps < 1)
        return (c1);
    factor = (double)steps / total_steps;
    color.r = c1.r * (1.0 - factor) + c2.r * factor;
    color.g = c1.g * (1.0 - factor) + c2.g * factor;
    color.b = c1.b * (1.0 - factor) + c2.b * factor;
    color.a = c1.a * (1.0 - factor) + c2.a * factor;
    return color;
}
