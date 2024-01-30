#include "../headers/draw_unsafe.h"
#include "../headers/maths.h"

Vector get_direction_unsafe(Vector v1, Vector v2)
{
    Vector dir;

    dir.x = v2.x - v1.x;
    dir.y = v2.y - v1.y;

    return dir;
}

void draw_point_unsafe(Texture* t, GLubyte color, int x, int y)
{
    int row;

    /* TODO: Remove after development -------------- */
    if (is_coord_out_of_bounds(t->width, x) 
        || is_coord_out_of_bounds(t->height, y))
    {
        printf("draw_point_unsafe: (%d, %d)\n", x, y);
        return;
    }
    /* --------------------------------------------- */

    x *= t->thickness;
    y *= t->thickness;
    
    for (row = 0; row < t->thickness; ++row)
        memset(t->buffer + ((y+row) * t->real_width + x), color, t->thickness);
    return;
}

void draw_line_unsafe(Texture* t, Vertex v1, Vertex v2)
{
    Vector dir = get_direction_unsafe(v1.coords, v2.coords);

    if (!dir.x && !dir.y)
    {
        draw_point_unsafe(t, v1.color, v1.coords.x, v1.coords.y);
    }
    else if (!dir.x)
    {
        if (dir.y > 0)
            draw_line_vertical_unsafe(t, v1, v2.coords.y);
        else
            draw_line_vertical_unsafe(t, v2, v1.coords.y);
    }
    else if (!dir.y)
    {
        if (dir.x > 0)
            draw_line_horizontal_unsafe(t, v1, v2.coords.x);
        else
            draw_line_horizontal_unsafe(t, v2, v1.coords.x);
    }
    else
    {
        draw_line_diagonal_unsafe(t, v1, dir);
    }
    return;
}

void draw_line_horizontal_unsafe(Texture* t, Vertex v, int last_x)
{
    int pixel_length = last_x - v.coords.x + 1;
    int row;

    v.coords.x *= t->thickness;
    v.coords.y *= t->thickness;
    pixel_length *= t->thickness;

    for (row = 0; row < t->thickness; ++row)
        memset(t->buffer + ((v.coords.y+row) * t->real_width + v.coords.x), 
            v.color, pixel_length);
    return;
}

void draw_line_vertical_unsafe(Texture* t, Vertex v, int last_y)
{
    while (v.coords.y <= last_y)
    {
        draw_point_unsafe(t, v.color, v.coords.x, v.coords.y);
        ++v.coords.y;
    }
    return;
}

void draw_line_diagonal_unsafe(Texture* t, Vertex v, Vector dir)
{
    int steps;
    VectorF coords, increment;

    steps = MAX(ABS(dir.x), ABS(dir.y));
    coords.x = v.coords.x;
    coords.y = v.coords.y;
    increment.x = dir.x / (float)steps;
    increment.y = dir.y / (float)steps;

    ++steps;

    while (steps-- > 0)
    {
        draw_point_unsafe(t, v.color, coords.x, coords.y);
        coords.x += increment.x;
        coords.y += increment.y;
    }
    return;
}

void draw_rectangle_unsafe(Texture* t, int full, Vertex v, int width, 
    int height)
{
    if (full)
        draw_rectangle_full_unsafe(t, v, width, height);
    else
        draw_rectangle_empty_unsafe(t, v, width, height);
    return;
}

void draw_rectangle_full_unsafe(Texture* t, Vertex v, int width, int height)
{
    Vector last_coords;
    last_coords.x = v.coords.x + width-1;
    last_coords.y = v.coords.y + height-1;

    while (v.coords.y <= last_coords.y)
    {
        draw_line_horizontal_unsafe(t, v, last_coords.x);
        ++v.coords.y;
    }
    return;
}

void draw_rectangle_empty_unsafe(Texture* t, Vertex v, int width, int height)
{
    int tmp;
    Vector last_coords;
    last_coords.x = v.coords.x + width-1;
    last_coords.y = v.coords.y + height-1;

    /* Draw bottom line */
    draw_line_horizontal_unsafe(t, v, last_coords.x);

    /* Draw left column */
    draw_line_vertical_unsafe(t, v, last_coords.y);

    /* Draw top line */
    tmp = v.coords.y;
    v.coords.y = last_coords.y;
    draw_line_horizontal_unsafe(t, v, last_coords.x);

    /* Draw right column */
    v.coords.x = last_coords.x;
    v.coords.y = tmp;
    draw_line_vertical_unsafe(t, v, last_coords.y);
    return;
}

void draw_circle_unsafe(Texture* t, int full, Vertex v, int radius)
{
    if (full)
        draw_circle_full_unsafe(t, v, radius);
    else
        draw_circle_empty_unsafe(t, v, radius);
    return;
}

void draw_circle_full_unsafe(Texture* t, Vertex center, int radius)
{
    /* Midpoint Circle Drawing algorithm */
    int x = radius;
    int y = 0;
    int decision = 1 - radius;
    int last_coord;
    Vertex v;
    v.color = center.color;

    while (y <= x)
    {
        /**/
        v.coords.x = center.coords.x - x;
        last_coord = center.coords.x + x;

        v.coords.y = center.coords.y - y;
        draw_line_horizontal_unsafe(t, v, last_coord);

        v.coords.y = center.coords.y + y;
        draw_line_horizontal_unsafe(t, v, last_coord);

        /**/
        v.coords.x = center.coords.x - y;
        last_coord = center.coords.x + y;

        v.coords.y = center.coords.y - x;
        draw_line_horizontal_unsafe(t, v, last_coord);

        v.coords.y = center.coords.y + x;
        draw_line_horizontal_unsafe(t, v, last_coord);

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

void draw_circle_empty_unsafe(Texture* t, Vertex center, int radius)
{
    /* Midpoint Circle Drawing algorithm */
    int x = radius;
    int y = 0;
    int decision = 1 - radius;
    Vector point;

    while (y <= x)
    {
        /**/
        point.y = center.coords.y - y;

        point.x = center.coords.x - x;
        draw_point_unsafe(t, center.color, point.x, point.y);

        point.x = center.coords.x + x;
        draw_point_unsafe(t, center.color, point.x, point.y);

        /**/
        point.y = center.coords.y + y;

        point.x = center.coords.x - x;
        draw_point_unsafe(t, center.color, point.x, point.y);

        point.x = center.coords.x + x;
        draw_point_unsafe(t, center.color, point.x, point.y);

        /**/
        point.y = center.coords.y - x;

        point.x = center.coords.x - y;
        draw_point_unsafe(t, center.color, point.x, point.y);

        point.x = center.coords.x + y;
        draw_point_unsafe(t, center.color, point.x, point.y);

        /**/
        point.y = center.coords.y + x;

        point.x = center.coords.x - y;
        draw_point_unsafe(t, center.color, point.x, point.y);

        point.x = center.coords.x + y;
        draw_point_unsafe(t, center.color, point.x, point.y);

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

void draw_shape_unsafe(Texture* t, int full, Vertex arr[], int len)
{
    int i;

    if (full)
        draw_shape_full_unsafe(t, arr, len);
    else
    {
        for (i = 0; i < len; ++i)
            draw_line_unsafe(t, arr[i], arr[(i + 1) % len]);
    }
    return;
}

void draw_shape_full_unsafe(Texture* t, Vertex arr[], int len)
{
    /* Scanline Fill algorithm */
    int i, j;
    int y, x1, x2, x_intersection;
    int ymin, ymax;
    Vertex v;
    v.color = arr[0].color;

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
            v.coords.x = x1;
            v.coords.y = y;
            draw_line_horizontal_unsafe(t, v, x2);
        }
    }
    return;
}

