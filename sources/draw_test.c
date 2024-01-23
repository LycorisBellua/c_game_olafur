#include "../headers/draw_test.h"
#include "../headers/draw.h"
#include "../headers/colors.h"
#include "../headers/maths.h"

void draw_test_corners(Texture* t)
{
    Vertex v;

    /* Bottom left */
    v.coords.x = 0;
    v.coords.y = 0;
    v.color = colors[COLOR_WHITE];
    draw_point(t, v.color, v.coords.x, v.coords.y);

    /* Top left */
    v.coords.x = 0;
    v.coords.y = get_coord_y(t, 1);
    v.color = colors[COLOR_RED];
    draw_point(t, v.color, v.coords.x, v.coords.y);

    /* Top right */
    v.coords.x = get_coord_x(t, 1);
    v.coords.y = get_coord_y(t, 1);
    v.color = colors[COLOR_GREEN];
    draw_point(t, v.color, v.coords.x, v.coords.y);

    /* Bottom right */
    v.coords.x = get_coord_x(t, 1);
    v.coords.y = 0;
    v.color = colors[COLOR_BLUE];
    draw_point(t, v.color, v.coords.x, v.coords.y);
    return;
}

void draw_test_center(Texture* t)
{
    Vertex v;

    /* Bottom left */
    v.coords.x = get_coord_x(t, 0.5f);
    v.coords.y = get_coord_y(t, 0.5f);
    v.color = colors[COLOR_WHITE];
    draw_point(t, v.color, v.coords.x, v.coords.y);

    /* Top left */
    v.coords.x = get_coord_x(t, 0.5f);
    v.coords.y = get_coord_y(t, 0.5f) + 1;
    v.color = colors[COLOR_RED];
    draw_point(t, v.color, v.coords.x, v.coords.y);

    /* Top right */
    v.coords.x = get_coord_x(t, 0.5f) + 1;
    v.coords.y = get_coord_y(t, 0.5f) + 1;
    v.color = colors[COLOR_GREEN];
    draw_point(t, v.color, v.coords.x, v.coords.y);

    /* Bottom right */
    v.coords.x = get_coord_x(t, 0.5f) + 1;
    v.coords.y = get_coord_y(t, 0.5f);
    v.color = colors[COLOR_BLUE];
    draw_point(t, v.color, v.coords.x, v.coords.y);
    return;
}

void draw_test_lines(Texture* t)
{
    Vertex v1, v2;

    v1.color = colors[COLOR_GREEN];
    v2.color = colors[COLOR_GREEN];

    v1.coords.x = get_coord_x(t, 0.5f) + 5;
    v1.coords.y = get_coord_y(t, 0.5f) + 5;
    v2.coords.x = v1.coords.x + 200;
    v2.coords.y = v1.coords.y + 100;
    draw_line(t, v1, v2);

    v1.color = colors[COLOR_RED];
    v2.color = colors[COLOR_RED];

    v1.coords.x -= NORMALIZE(v2.coords.x - v1.coords.x)*2;
    v1.coords.y -= NORMALIZE(v2.coords.y - v1.coords.y)*2;
    if (!is_coord_out_of_bounds(t->width, v1.coords.x) 
        && !is_coord_out_of_bounds(t->height, v1.coords.y))
        draw_point(t, v1.color, v1.coords.x, v1.coords.y);

    v2.coords.x -= NORMALIZE(v1.coords.x - v2.coords.x)*2;
    v2.coords.y -= NORMALIZE(v1.coords.y - v2.coords.y)*2;
    if (!is_coord_out_of_bounds(t->width, v2.coords.x) 
        && !is_coord_out_of_bounds(t->height, v2.coords.y))
        draw_point(t, v2.color, v2.coords.x, v2.coords.y);
    return;
}

void draw_test_rectangles(Texture* t)
{
    Vertex v1, v2;

    v1.coords.x = get_coord_x(t, 0.1f) - 1;
    v1.coords.y = get_coord_y(t, 0.33f) - 1;
    v1.color = colors[COLOR_WHITE];
    draw_rectangle(t, 1, v1, 100, 100);

    v2.coords.x = get_coord_x(t, 0.1f);
    v2.coords.y = get_coord_y(t, 0.33f) - 2;
    v2.color = colors[COLOR_RED];
    draw_rectangle(t, 0, v2, 100, 100);

    return;
}

void draw_test_circles(Texture* t)
{
    int radius = 100;
    Vertex v;
    v.color = colors[COLOR_BLUE];
    v.coords.x = get_coord_x(t, 0.7f);
    v.coords.y = get_coord_y(t, 0.4f);
    draw_circle(t, 1, v, radius);

    v.color = colors[COLOR_WHITE];
    draw_circle(t, 0, v, radius);
    return;
}

void draw_test_shapes(Texture* t)
{
    int full = 1;

    /* Convex (triangle) */
    Vertex v[3];
    v[0].color = colors[0];
    v[1].color = v[0].color;
    v[2].color = v[0].color;

    v[0].coords.x = get_coord_x(t, 0.25f);
    v[0].coords.y = get_coord_y(t, 0.25f);
    v[1].coords.x = get_coord_x(t, 0.50f);
    v[1].coords.y = get_coord_y(t, 0.75f);
    v[2].coords.x = get_coord_x(t, 0.75f);
    v[2].coords.y = get_coord_y(t, 0.25f);

    /* Concave and self-intercepting */
    /*
    Vertex v[5];
    v[0].color = colors[0];
    v[1].color = v[0].color;
    v[2].color = v[0].color;
    v[3].color = v[0].color;
    v[4].color = v[0].color;

    v[0].coords.x = get_coord_x(t, 0.25f);
    v[0].coords.y = get_coord_y(t, 0.25f);
    v[1].coords.x = get_coord_x(t, 0.50f);
    v[1].coords.y = get_coord_y(t, 0.75f);
    v[2].coords.x = get_coord_x(t, 0.30f);
    v[2].coords.y = get_coord_y(t, 0.44f);
    v[3].coords.x = get_coord_x(t, 0.75f);
    v[3].coords.y = get_coord_y(t, 0.25f);
    v[4].coords.x = get_coord_x(t, 0.50f);
    v[4].coords.y = get_coord_y(t, 0.10f);
    */

    draw_shape(t, full, v, sizeof(v)/sizeof(v[0]));
    return;
}

void draw_test_gradient(Texture* t)
{
    Vertex v;
    GLubyte color[4];
    color[2] = 255/2;
    color[3] = 255;
    v.color = color;

    for (v.coords.y = 0; v.coords.y < t->height; ++v.coords.y)
    {
        for (v.coords.x = 0; v.coords.x < t->width; ++v.coords.x)
        {
            color[0] = v.coords.x * 255 / t->width;
            color[1] = v.coords.y * 255 / t->height;
            draw_point(t, v.color, v.coords.x, v.coords.y);
        }
    }
    return;
}

void draw_test_gradient_line(Texture* t)
{
    int i;
    int dist = 200;
    int half_dist = dist/2;
    float perc = 0;
    GLubyte first_color[4] = {0};
    GLubyte second_color[4] = {0};
    GLubyte color_change[4] = {0};
    Vector v;
    v.x = get_coord_x(t, 0.1f);
    v.y = get_coord_x(t, 0.4f);

    first_color[0] = colors[COLOR_RED][0];
    first_color[1] = colors[COLOR_RED][1];
    first_color[2] = colors[COLOR_RED][2];
    first_color[3] = colors[COLOR_RED][3];

    second_color[0] = colors[COLOR_GREEN][0];
    second_color[1] = colors[COLOR_GREEN][1];
    second_color[2] = colors[COLOR_GREEN][2];
    second_color[3] = colors[COLOR_GREEN][3];

    if (first_color[0] - second_color[0] 
        || first_color[1] - second_color[1] 
        || first_color[2] - second_color[2] 
        || first_color[3] - second_color[3])
    {
        perc = 100.0f / dist;

        color_change[0] = (second_color[0] - first_color[0]) / 50.0f * perc;
        color_change[1] = (second_color[1] - first_color[1]) / 50.0f * perc;
        color_change[2] = (second_color[2] - first_color[2]) / 50.0f * perc;
        color_change[3] = (second_color[3] - first_color[3]) / 50.0f * perc;
    }
    for (i = 0; i < half_dist; ++i)
    {
        draw_point(t, first_color, v.x, v.y);
        ++v.x;
        first_color[0] = CLAMP(first_color[0] + color_change[0], 0, 255);
        first_color[1] = CLAMP(first_color[1] + color_change[1], 0, 255);
        first_color[2] = CLAMP(first_color[2] + color_change[2], 0, 255);
        first_color[3] = CLAMP(first_color[3] + color_change[3], 0, 255);
    }

    if (perc > 0)
    {
        color_change[0] = (first_color[0] - second_color[0]) / 50.0f * perc;
        color_change[1] = (first_color[1] - second_color[1]) / 50.0f * perc;
        color_change[2] = (first_color[2] - second_color[2]) / 50.0f * perc;
        color_change[3] = (first_color[3] - second_color[3]) / 50.0f * perc;
    }
    for (; i < dist; ++i)
    {
        draw_point(t, first_color, v.x, v.y);
        ++v.x;
        first_color[0] = CLAMP(first_color[0] - color_change[0], 0, 255);
        first_color[1] = CLAMP(first_color[1] - color_change[1], 0, 255);
        first_color[2] = CLAMP(first_color[2] - color_change[2], 0, 255);
        first_color[3] = CLAMP(first_color[3] - color_change[3], 0, 255);
    }
    return;
}

