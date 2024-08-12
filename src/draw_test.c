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
    Vector size;

    v1.coords.x = get_coord_x(t, 0.1f);
    v1.coords.y = get_coord_y(t, 0.33f);
    v1.color = get_color_rgba(255, 255, 255, 255);
    size.x = 100;
    size.y = 100;
    draw_rectangle_full(t, v1, size);

    v2.coords.x = get_coord_x(t, 0.1f) + 1;
    v2.coords.y = get_coord_y(t, 0.33f) + 1;
    v2.color = get_color_rgba(255, 0, 0, 255);
    draw_rectangle(t, v2, size);
    return;
}

void draw_test_circles(Texture* t)
{
    int radius = 100;
    Color edge_color = get_color_rgba(0, 0, 0, 255);
    Vertex v;
    v.color = get_color_rgba(0, 0, 255, 255);
    v.coords.x = get_coord_x(t, 0.75f);
    v.coords.y = get_coord_y(t, 0.5f);
    draw_circle_full_grad(t, v, radius, edge_color);

    v.color = get_color_rgba(148, 53, 170, 255);
    v.coords.x = get_coord_x(t, 0.25f);
    draw_circle_full_grad(t, v, radius, edge_color);
    return;
}

void draw_test_shapes(Texture* t)
{
    /* Convex (triangle) */
    /*
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
    */

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

    /* Concave */
    /* Star Shape */
    Vertex v[10];
    v[0].color = get_color_rgba(255, 255, 255, 255); // White
    v[1].color = get_color_rgba(255, 0, 0, 255);     // Red
    v[2].color = get_color_rgba(0, 255, 0, 255);     // Green
    v[3].color = get_color_rgba(0, 0, 255, 255);     // Blue
    v[4].color = get_color_rgba(255, 255, 0, 255);   // Yellow
    v[5].color = get_color_rgba(0, 255, 255, 255);   // Cyan
    v[6].color = get_color_rgba(255, 0, 255, 255);   // Magenta
    v[7].color = get_color_rgba(128, 128, 128, 255); // Gray
    v[8].color = get_color_rgba(255, 165, 0, 255);   // Orange
    v[9].color = get_color_rgba(75, 0, 130, 255);    // Indigo

    v[0].coords.x = get_coord_x(t, 0.50f);  // Top point (outer)
    v[0].coords.y = get_coord_y(t, 0.10f);

    v[1].coords.x = get_coord_x(t, 0.60f);  // Right inner point
    v[1].coords.y = get_coord_y(t, 0.30f);

    v[2].coords.x = get_coord_x(t, 0.80f);  // Right outer point
    v[2].coords.y = get_coord_y(t, 0.35f);

    v[3].coords.x = get_coord_x(t, 0.65f);  // Bottom right inner point
    v[3].coords.y = get_coord_y(t, 0.55f);

    v[4].coords.x = get_coord_x(t, 0.75f);  // Bottom right outer point
    v[4].coords.y = get_coord_y(t, 0.80f);

    v[5].coords.x = get_coord_x(t, 0.50f);  // Bottom point (outer)
    v[5].coords.y = get_coord_y(t, 0.65f);

    v[6].coords.x = get_coord_x(t, 0.25f);  // Bottom left outer point
    v[6].coords.y = get_coord_y(t, 0.80f);

    v[7].coords.x = get_coord_x(t, 0.35f);  // Bottom left inner point
    v[7].coords.y = get_coord_y(t, 0.55f);

    v[8].coords.x = get_coord_x(t, 0.20f);  // Left outer point
    v[8].coords.y = get_coord_y(t, 0.35f);

    v[9].coords.x = get_coord_x(t, 0.40f);  // Left inner point
    v[9].coords.y = get_coord_y(t, 0.30f);

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

    draw_shape_full(t, v, sizeof(v)/sizeof(v[0]));
    /*
    Vertex intersection;
    intersection.coords.x = get_coord_x(t, 0.337f);
    intersection.coords.y = get_coord_y(t, 0.424f);
    intersection.color = get_color_rgba(127, 255, 127, 255);
    draw_point(t, intersection.color, intersection.coords.x, intersection.coords.y);
    */
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
