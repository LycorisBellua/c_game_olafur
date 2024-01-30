#include "../headers/draw_test.h"
#include "../headers/draw.h"
#include "../headers/colors.h"
#include "../headers/maths.h"
#include "../headers/game.h"

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

    v1.coords.x = get_coord_x(t, 0.5f) + 5 + global_x;
    v1.coords.y = get_coord_y(t, 0.5f) + 5 + global_y;
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

    v1.coords.x = get_coord_x(t, 0.1f) - 1 + global_x;
    v1.coords.y = get_coord_y(t, 0.33f) - 1 + global_y;
    v1.color = colors[COLOR_WHITE];
    draw_rectangle(t, 1, v1, 100, 100);

    v2.coords.x = get_coord_x(t, 0.1f) + global_x;
    v2.coords.y = get_coord_y(t, 0.33f) - 2 + global_y;
    v2.color = colors[COLOR_RED];
    draw_rectangle(t, 0, v2, 100, 100);
    return;
}

void draw_test_circles(Texture* t)
{
    int radius = 100;
    Vertex v;
    v.color = colors[COLOR_BLUE];
    v.coords.x = get_coord_x(t, 0.7f) + global_x;
    v.coords.y = get_coord_y(t, 0.4f) + global_y;
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
    v[0].color = get_color_from_hex_code("#C41E3A");
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

void draw_test_gradient_line(Texture* t)
{
    int i;
    const int dist = 200;
    const int half_dist = dist/2;
    const float perc = 100.0f / dist;
    GLubyte first_color, second_color, color_change;
    Vector v;
    v.x = get_coord_x(t, 0.1f) + global_x;
    v.y = get_coord_x(t, 0.4f) + global_y;

    first_color = colors[COLOR_RED];
    second_color = colors[COLOR_GREEN];
    color_change = 0;

    set_red_channel(&color_change, (get_red_channel(second_color) 
        - get_red_channel(first_color)) / 2 * perc + 0.5f);
    set_green_channel(&color_change, (get_green_channel(second_color) 
        - get_green_channel(first_color)) / 2 * perc + 0.5f);
    set_blue_channel(&color_change, (get_blue_channel(second_color) 
        - get_blue_channel(first_color)) / 2 * perc + 0.5f);
    for (i = 0; i < half_dist; ++i)
    {
        draw_point(t, first_color, v.x, v.y);
        ++v.x;

        set_red_channel(&first_color, CLAMP(get_red_channel(first_color) 
            + get_red_channel(color_change), 0, MAX_RED));
        set_green_channel(&first_color, CLAMP(get_green_channel(first_color) 
            + get_green_channel(color_change), 0, MAX_GREEN));
        set_blue_channel(&first_color, CLAMP(get_blue_channel(first_color) 
            + get_blue_channel(color_change), 0, MAX_BLUE));
    }

    set_red_channel(&color_change, (get_red_channel(first_color) 
        - get_red_channel(second_color)) / 2 * perc + 0.5f);
    set_green_channel(&color_change, (get_green_channel(first_color) 
        - get_green_channel(second_color)) / 2 * perc + 0.5f);
    set_blue_channel(&color_change, (get_blue_channel(first_color) 
        - get_blue_channel(second_color)) / 2 * perc + 0.5f);
    for (; i < dist; ++i)
    {
        draw_point(t, first_color, v.x, v.y);
        ++v.x;

        set_red_channel(&first_color, CLAMP(get_red_channel(first_color) 
            - get_red_channel(color_change), 0, MAX_RED));
        set_green_channel(&first_color, CLAMP(get_green_channel(first_color) 
            - get_green_channel(color_change), 0, MAX_GREEN));
        set_blue_channel(&first_color, CLAMP(get_blue_channel(first_color) 
            - get_blue_channel(color_change), 0, MAX_BLUE));
    }
    return;
}

void draw_test_gradient(Texture* t)
{
    Vertex v;

    v.color = 0;
    set_blue_channel(&v.color, MAX_BLUE/2);

    for (v.coords.y = 0; v.coords.y < t->height; ++v.coords.y)
    {
        for (v.coords.x = 0; v.coords.x < t->width; ++v.coords.x)
        {
            set_red_channel(&v.color, v.coords.x * MAX_RED / t->width);
            set_green_channel(&v.color, v.coords.y * MAX_GREEN / t->height);
            draw_point(t, v.color, v.coords.x, v.coords.y);
        }
    }
    return;
}

void draw_palette(Texture* t)
{
    const int root = 16; /* because 256 colors */
    const int square_size_x = t->width / root;
    const int square_size_y = t->height / root;
    int i, col, row;
    Vertex v;

    col = 0;
    row = 0;
    for (i = 0; i < NBR_COLORS; ++i)
    {
        v.coords.x = col * square_size_x;
        v.coords.y = row * square_size_y;
        v.color = colors[i];
        draw_rectangle(t, 1, v, square_size_x, square_size_y);

        v.color = colors[COLOR_BLACK];
        draw_rectangle(t, 0, v, square_size_x, square_size_y);

        ++col;
        if (!(col % root))
        {
            col = 0;
            ++row;
        }
    }
    return;
}

