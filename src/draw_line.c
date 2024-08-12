#include "cimmerian.h"

static Color get_lerp_color(Color c1, Color c2, int steps, int total_steps);

void draw_line(Texture* t, Vertex v1, Vertex v2)
{
    int steps;
    int total_steps;
    Vector dir;
    VectorF coords;
    VectorF increment;

    dir = get_direction(v1.coords, v2.coords);
    steps = max(abs(dir.x), abs(dir.y));
    coords.x = v1.coords.x;
    coords.y = v1.coords.y;
    increment.x = dir.x / (double)steps;
    increment.y = dir.y / (double)steps;
    ++steps;
    total_steps = steps;
    while (steps-- > 0)
    {
        draw_point(t, get_lerp_color(v1.color, v2.color, steps, total_steps),
            coords.x, coords.y);
        coords.x += increment.x;
        coords.y += increment.y;
    }
    return;
}

static Color get_lerp_color(Color c1, Color c2, int steps, int total_steps)
{
    Color color;
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
