#include "cimmerian.h"

void draw_test(void)
{
    draw_test_shapes(man.tex[man.curr_tex]);
    return ;
}

void draw_test_corners(Texture* t)
{
    Vertex v;

    /* Top left */
    v.coords.x = 0;
    v.coords.y = 0;
    v.color = get_color_rgba(255, 255, 255, 255);
    draw_point(t, v.color, v.coords.x, v.coords.y);

    /* Top right */
    v.coords.x = get_coord_x(t, 1);
    v.coords.y = 0;
    v.color = get_color_rgba(255, 0, 0, 255);
    draw_point(t, v.color, v.coords.x, v.coords.y);

    /* Bottom right */
    v.coords.x = get_coord_x(t, 1);
    v.coords.y = get_coord_y(t, 1);
    v.color = get_color_rgba(0, 255, 0, 255);
    draw_point(t, v.color, v.coords.x, v.coords.y);

    /* Bottom left */
    v.coords.x = 0;
    v.coords.y = get_coord_y(t, 1);
    v.color = get_color_rgba(0, 0, 255, 255);
    draw_point(t, v.color, v.coords.x, v.coords.y);
    return;
}

void draw_test_center(Texture* t)
{
    Vertex v;

    /* Top left */
    v.coords.x = get_coord_x(t, 0.5f);
    v.coords.y = get_coord_y(t, 0.5f);
    v.color = get_color_rgba(255, 255, 255, 255);
    draw_point(t, v.color, v.coords.x, v.coords.y);

    /* Top right */
    v.coords.x = get_coord_x(t, 0.5f) + 1;
    v.coords.y = get_coord_y(t, 0.5f);
    v.color = get_color_rgba(255, 0, 0, 255);
    draw_point(t, v.color, v.coords.x, v.coords.y);

    /* Bottom right */
    v.coords.x = get_coord_x(t, 0.5f) + 1;
    v.coords.y = get_coord_y(t, 0.5f) + 1;
    v.color = get_color_rgba(0, 255, 0, 255);
    draw_point(t, v.color, v.coords.x, v.coords.y);

    /* Bottom left */
    v.coords.x = get_coord_x(t, 0.5f);
    v.coords.y = get_coord_y(t, 0.5f) + 1;
    v.color = get_color_rgba(0, 0, 255, 255);
    draw_point(t, v.color, v.coords.x, v.coords.y);
    return;
}

void draw_test_lines(Texture* t)
{
    Vertex v1, v2;

    v1.color = get_color_rgba(0, 255, 0, 255);
    v2.color = get_color_rgba(0, 255, 0, 255);

    v1.coords.x = get_coord_x(t, 0.5f) + 5;
    v1.coords.y = get_coord_y(t, 0.5f) + 5;
    v2.coords.x = v1.coords.x + 200;
    v2.coords.y = v1.coords.y + 100;
    draw_line(t, v1, v2);

    v1.color = get_color_rgba(255, 0, 0, 255);
    v2.color = get_color_rgba(255, 0, 0, 255);

    v1.coords.x -= normalize(v2.coords.x - v1.coords.x)*2;
    v1.coords.y -= normalize(v2.coords.y - v1.coords.y)*2;
    draw_point(t, v1.color, v1.coords.x, v1.coords.y);

    v2.coords.x -= normalize(v1.coords.x - v2.coords.x)*2;
    v2.coords.y -= normalize(v1.coords.y - v2.coords.y)*2;
    draw_point(t, v2.color, v2.coords.x, v2.coords.y);
    return;
}

void draw_test_rectangles(Texture* t)
{
    Vertex v1, v2;

    v1.coords.x = get_coord_x(t, 0.1f);
    v1.coords.y = get_coord_y(t, 0.33f);
    v1.color = get_color_rgba(255, 255, 255, 255);
    draw_rectangle(t, 1, v1, 100, 100);

    v2.coords.x = get_coord_x(t, 0.1f) + 2;
    v2.coords.y = get_coord_y(t, 0.33f) + 2;
    v2.color = get_color_rgba(255, 0, 0, 255);
    draw_rectangle(t, 0, v2, 100, 100);
    return;
}

void draw_test_circles(Texture* t)
{
    int radius = 100;
    Vertex v;
    v.color = get_color_rgba(0, 0, 255, 255);
    v.coords.x = get_coord_x(t, 0.7f);
    v.coords.y = get_coord_y(t, 0.4f);
    draw_circle(t, 1, v, radius);

    v.color = get_color_rgba(255, 255, 255, 255);
    draw_circle(t, 0, v, radius);
    return;
}

void draw_test_shapes(Texture* t)
{
    int full = 1;

    /* Convex (triangle) */
    Vertex v[3];
    v[0].color = get_color_rgba(255, 0, 0, 255);
    v[1].color = get_color_rgba(0, 255, 0, 255);
    v[2].color = get_color_rgba(0, 0, 255, 255);

    v[0].coords.x = get_coord_x(t, 0.50f);
    v[0].coords.y = get_coord_y(t, 0.25f);
    v[1].coords.x = get_coord_x(t, 0.75f);
    v[1].coords.y = get_coord_y(t, 0.75f);
    v[2].coords.x = get_coord_x(t, 0.25f);
    v[2].coords.y = get_coord_y(t, 0.75f);

    /* Rectangle */
    /*
    Vertex v[4];
    v[0].color = get_color_rgba(255, 0, 0, 255);
    v[1].color = get_color_rgba(0, 255, 0, 255);
    v[2].color = get_color_rgba(0, 0, 255, 255);
    v[3].color = get_color_rgba(255, 255, 0, 255);

    v[0].coords.x = get_coord_x(t, 0.25f);
    v[0].coords.y = get_coord_y(t, 0.25f);
    v[1].coords.x = get_coord_x(t, 0.75f);
    v[1].coords.y = get_coord_y(t, 0.25f);
    v[2].coords.x = get_coord_x(t, 0.75f);
    v[2].coords.y = get_coord_y(t, 0.75f);
    v[3].coords.x = get_coord_x(t, 0.25f);
    v[3].coords.y = get_coord_y(t, 0.75f);
    */

    /* Concave and self-intercepting */
    /*
    Vertex v[5];
    v[0].color = get_color_rgba(255, 255, 255, 255);
    v[1].color = get_color_rgba(255, 0, 0, 255);
    v[2].color = get_color_rgba(0, 255, 0, 255);
    v[3].color = get_color_rgba(0, 0, 255, 255);
    v[4].color = get_color_rgba(255, 255, 0, 255);

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

    v.color.b = 255/2;
    v.color.a = 255;
    v.coords.y = 0;
    while (v.coords.y < t->height)
    {
        v.coords.x = 0;
        while (v.coords.x < t->width)
        {
            v.color.r = 255 * v.coords.y / t->height;
            v.color.g = 255 * v.coords.x / t->width;
            draw_point(t, v.color, v.coords.x, v.coords.y);
            ++v.coords.x;
        }
        ++v.coords.y;
    }
    return;
}
