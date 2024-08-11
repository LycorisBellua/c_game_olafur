#include "cimmerian.h"

static Color interpolate_color(Color start, Color end, double ratio);

/* Bresenham/Midpoint Circle Drawing algorithm */
void draw_circle(Texture* t, Vertex center, int radius)
{
    int x;
    int y;
    int decision;
    Vector point;

    x = radius;
    y = 0;
    decision = 1 - radius;
    while (y <= x)
    {
        point.y = center.coords.y - y;
        point.x = center.coords.x - x;
        draw_point(t, center.color, point.x, point.y);
        point.x = center.coords.x + x;
        draw_point(t, center.color, point.x, point.y);
        point.y = center.coords.y + y;
        point.x = center.coords.x - x;
        draw_point(t, center.color, point.x, point.y);
        point.x = center.coords.x + x;
        draw_point(t, center.color, point.x, point.y);
        point.y = center.coords.y - x;
        point.x = center.coords.x - y;
        draw_point(t, center.color, point.x, point.y);
        point.x = center.coords.x + y;
        draw_point(t, center.color, point.x, point.y);
        point.y = center.coords.y + x;
        point.x = center.coords.x - y;
        draw_point(t, center.color, point.x, point.y);
        point.x = center.coords.x + y;
        draw_point(t, center.color, point.x, point.y);
        if (decision <= 0)
            decision += 2 * ++y + 1;
        else
            decision += 2 * (++y - --x) + 1;
    }
    return;
}

/* Bresenham/Midpoint Circle Drawing algorithm */
void draw_circle_full(Texture* t, Vertex center, int radius)
{
    int x;
    int y;
    int decision;
    Vertex v1;
    Vertex v2;

    x = radius;
    y = 0;
    decision = 1 - radius;
    v1.color = center.color;
    v2.color = v1.color;
    while (y <= x)
    {
        v1.coords.x = center.coords.x - x;
        v2.coords.x = center.coords.x + x;
        v1.coords.y = center.coords.y - y;
        v2.coords.y = v1.coords.y;
        draw_line(t, v1, v2);
        v1.coords.y = center.coords.y + y;
        v2.coords.y = v1.coords.y;
        draw_line(t, v1, v2);
        v1.coords.x = center.coords.x - y;
        v2.coords.x = center.coords.x + y;
        v1.coords.y = center.coords.y - x;
        v2.coords.y = v1.coords.y;
        draw_line(t, v1, v2);
        v1.coords.y = center.coords.y + x;
        v2.coords.y = v1.coords.y;
        draw_line(t, v1, v2);
        if (decision <= 0)
            decision += 2 * ++y + 1;
        else
            decision += 2 * (++y - --x) + 1;
    }
    return;
}

void draw_circle_full_grad(Texture* t, Vertex center, int radius, Color edge)
{
    int x;
    int y;
    int distance_squared;
    double distance;
    double linear_ratio;
    double adjusted_ratio;
    Color current_color;

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
                current_color = interpolate_color(center.color, edge, adjusted_ratio);
                draw_point(t, current_color, center.coords.x + x, center.coords.y + y);
            }
            ++x;
        }
        ++y;
    }
}

static Color interpolate_color(Color start, Color end, double ratio)
{
    Color result;

    result.r = (1 - ratio) * start.r + ratio * end.r;
    result.g = (1 - ratio) * start.g + ratio * end.g;
    result.b = (1 - ratio) * start.b + ratio * end.b;
    result.a = (1 - ratio) * start.a + ratio * end.a;
    return result;
}
