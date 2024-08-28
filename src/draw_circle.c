#include "cimmerian.h"

static t_color interpolate_color(t_color start, t_color end, double ratio);

/* Bresenham/Midpoint Circle Drawing algorithm */
void draw_circle(t_frame* f, t_vert center, int radius)
{
    int x;
    int y;
    int decision;
    t_ivec2 point;

    x = radius;
    y = 0;
    decision = 1 - radius;
    while (y <= x)
    {
        point.y = center.coord.y - y;
        point.x = center.coord.x - x;
        draw_point(f, center.color, point.x, point.y);
        point.x = center.coord.x + x;
        draw_point(f, center.color, point.x, point.y);
        point.y = center.coord.y + y;
        point.x = center.coord.x - x;
        draw_point(f, center.color, point.x, point.y);
        point.x = center.coord.x + x;
        draw_point(f, center.color, point.x, point.y);
        point.y = center.coord.y - x;
        point.x = center.coord.x - y;
        draw_point(f, center.color, point.x, point.y);
        point.x = center.coord.x + y;
        draw_point(f, center.color, point.x, point.y);
        point.y = center.coord.y + x;
        point.x = center.coord.x - y;
        draw_point(f, center.color, point.x, point.y);
        point.x = center.coord.x + y;
        draw_point(f, center.color, point.x, point.y);
        if (decision <= 0)
            decision += 2 * ++y + 1;
        else
            decision += 2 * (++y - --x) + 1;
    }
    return;
}

/* Bresenham/Midpoint Circle Drawing algorithm */
void draw_circle_full(t_frame* f, t_vert center, int radius)
{
    int x;
    int y;
    int decision;
    t_vert v1;
    t_vert v2;

    x = radius;
    y = 0;
    decision = 1 - radius;
    v1.color = center.color;
    v2.color = v1.color;
    while (y <= x)
    {
        v1.coord.x = center.coord.x - x;
        v2.coord.x = center.coord.x + x;
        v1.coord.y = center.coord.y - y;
        v2.coord.y = v1.coord.y;
        draw_line(f, v1, v2);
        v1.coord.y = center.coord.y + y;
        v2.coord.y = v1.coord.y;
        draw_line(f, v1, v2);
        v1.coord.x = center.coord.x - y;
        v2.coord.x = center.coord.x + y;
        v1.coord.y = center.coord.y - x;
        v2.coord.y = v1.coord.y;
        draw_line(f, v1, v2);
        v1.coord.y = center.coord.y + x;
        v2.coord.y = v1.coord.y;
        draw_line(f, v1, v2);
        if (decision <= 0)
            decision += 2 * ++y + 1;
        else
            decision += 2 * (++y - --x) + 1;
    }
    return;
}

void draw_circle_full_grad(t_frame* f, t_vert center, int radius, t_color edge)
{
    int x;
    int y;
    int distance_squared;
    double distance;
    double linear_ratio;
    double adjusted_ratio;
    t_color curr_c;

    y = -radius;
    while (y <= radius)
    {
        x = -radius;
        while (x <= radius)
        {
            distance_squared = x * x + y * y;
            if (distance_squared <= radius * radius)
            {
                distance = f_sqrt((double)distance_squared);
                linear_ratio = distance / (double)radius;
                adjusted_ratio = linear_ratio * linear_ratio;
                curr_c = interpolate_color(center.color, edge, adjusted_ratio);
                draw_point(f, curr_c, center.coord.x + x, center.coord.y + y);
            }
            ++x;
        }
        ++y;
    }
}

static t_color interpolate_color(t_color start, t_color end, double ratio)
{
    t_color result;

    result.r = (1 - ratio) * start.r + ratio * end.r;
    result.g = (1 - ratio) * start.g + ratio * end.g;
    result.b = (1 - ratio) * start.b + ratio * end.b;
    result.a = (1 - ratio) * start.a + ratio * end.a;
    return result;
}
