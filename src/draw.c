#include "cimmerian.h"

static void draw_rectangle_full(Texture* t, Vertex v, int width, int height);
static void draw_rectangle_empty(Texture* t, Vertex v, int width, int height);
static void draw_circle_full(Texture* t, Vertex center, int radius);
static void draw_circle_empty(Texture* t, Vertex center, int radius);
static void draw_shape_full(Texture* t, Vertex arr[], int len);

/*
    The coordinate parameters are individual ints and not a vector, so 
    that either a Vector or VectorF can be used. Of course, double coords 
    are converted into integers through being passed as arguments to this 
    function. double coords are needed for drawing diagonal lines.

    As a bonus, being able to pass values directly into draw_point() 
    instead of declaring a vector variable is neat. May be useful.
*/
void draw_point(Texture* t, Color color, int x, int y)
{
    int row;
    int col;

    if (is_coord_out_of_bounds(t->width, x) 
        || is_coord_out_of_bounds(t->height, y))
        return;

    x *= t->thickness;
    y *= t->thickness;
    
    for (row = 0; row < t->thickness; ++row)
    {
        for (col = 0; col < t->thickness; ++col)
        {
            GLubyte *p = t->buffer + 4 * ((y+row) * t->real_width + (x+col));
            p[0] = color.r;
            p[1] = color.g;
            p[2] = color.b;
        }
    }
    return;
}

void draw_line(Texture* t, Vertex v1, Vertex v2)
{
    Vector dir;
    int steps;
    VectorF coords, increment;

    dir = get_direction(v1.coords, v2.coords);
    steps = max(abs(dir.x), abs(dir.y));
    coords.x = v1.coords.x;
    coords.y = v1.coords.y;
    increment.x = dir.x / (double)steps;
    increment.y = dir.y / (double)steps;
    ++steps;

    while (steps-- > 0)
    {
        draw_point(t, v1.color, coords.x, coords.y);
        coords.x += increment.x;
        coords.y += increment.y;
    }
    return;
}

void draw_rectangle(Texture* t, int full, Vertex v, int width, int height)
{
    if (full)
        draw_rectangle_full(t, v, width, height);
    else
        draw_rectangle_empty(t, v, width, height);
    return;
}

void draw_circle(Texture* t, int full, Vertex v, int radius)
{
    if (full)
        draw_circle_full(t, v, radius);
    else
        draw_circle_empty(t, v, radius);
    return;
}

void draw_shape(Texture* t, int full, Vertex arr[], int len)
{
    int i;

    if (len < 3)
        return;

    if (full)
        draw_shape_full(t, arr, len);
    else
    {
        for (i = 0; i < len; ++i)
            draw_line(t, arr[i], arr[(i + 1) % len]);
    }
    return;
}

static void draw_rectangle_full(Texture* t, Vertex v, int width, int height)
{
    int target_y;
    Vertex v2;

    target_y = v.coords.y + height-1;
    v2.coords.x = v.coords.x + width-1;
    v2.coords.y = v.coords.y;
    v2.color = v.color;

    while (v.coords.y <= target_y)
    {
        draw_line(t, v, v2);
        ++v.coords.y;
        ++v2.coords.y;
    }
    return;
}

static void draw_rectangle_empty(Texture* t, Vertex v, int width, int height)
{
    Vertex v2;
    Vertex v3;
    Vertex v4;

    v2.color = v.color;
    v3.color = v.color;
    v4.color = v.color;

    v2.coords.x = v.coords.x + width-1;
    v2.coords.y = v.coords.y;
    v3.coords.x = v.coords.x;
    v3.coords.y = v.coords.y + height-1;
    v4.coords.x = v.coords.x + width-1;
    v4.coords.y = v.coords.y + height-1;

    /* Top */
    draw_line(t, v, v2);

    /* Right */
    draw_line(t, v2, v4);

    /* Bottom */
    draw_line(t, v3, v4);

    /* Left */
    draw_line(t, v, v3);
    return;
}

/* Midpoint Circle Drawing algorithm */
static void draw_circle_full(Texture* t, Vertex center, int radius)
{
    int x = radius;
    int y = 0;
    int decision = 1 - radius;
    Vertex v1;
    Vertex v2;
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

        ++y;
        if (decision <= 0)
        {
            decision += 2 * y + 1;
        }
        else
        {
            --x;
            decision += 2 * (y - x) + 1;
        }
    }
    return;
}

/* Midpoint Circle Drawing algorithm */
static void draw_circle_empty(Texture* t, Vertex center, int radius)
{
    int x = radius;
    int y = 0;
    int decision = 1 - radius;
    Vector point;

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

        ++y;
        if (decision <= 0)
        {
            decision += 2 * y + 1;
        }
        else
        {
            --x;
            decision += 2 * (y - x) + 1;
        }
    }
    return;
}

/* Scanline Fill algorithm */
static void draw_shape_full(Texture* t, Vertex arr[], int len)
{
    int i, j;
    int y, x1, x2, x_intersection;
    int ymin, ymax;
    Vertex v1;
    Vertex v2;
    v1.color = arr[0].color;
    v2.color = v1.color;

    ymin = arr[0].coords.y;
    ymax = arr[0].coords.y;
    for (i = 1; i < len; ++i)
    {
        if (arr[i].coords.y < ymin)
            ymin = arr[i].coords.y;
        if (arr[i].coords.y > ymax)
            ymax = arr[i].coords.y;
    }

    for (y = ymin; y <= ymax; ++y)
    {
        x1 = t->width-1;
        x2 = 0;

        for (i = 0; i < len; ++i)
        {
            j = (i + 1) % len;
            if ((arr[i].coords.y <= y && arr[j].coords.y > y) 
                || (arr[j].coords.y <= y && arr[i].coords.y > y))
            {
                x_intersection = (arr[i].coords.x * (arr[j].coords.y - y) 
                    + arr[j].coords.x * (y - arr[i].coords.y)) 
                    / (arr[j].coords.y - arr[i].coords.y);

                if (x_intersection < x1)
                    x1 = x_intersection;
                if (x_intersection > x2)
                    x2 = x_intersection;
            }
        }

        if (x1 <= x2)
        {
            v1.coords.x = x1;
            v2.coords.x = x2;
            v1.coords.y = y;
            v2.coords.y = y;
            draw_line(t, v1, v2);
        }
    }
    return;
}
