#include "cimmerian.h"

void draw_test(void)
{
    draw_test_shapes(man.tex[man.curr_tex]);
    draw_sprite(man.tex[man.curr_tex], man.font_default);
    return;
}

void draw_test_corners(t_tex* t)
{
    t_vert v;

    /* Top left */
    v.coord.x = 0;
    v.coord.y = 0;
    v.color = get_color_rgba(255, 255, 255, 255);
    draw_point(t, v.color, v.coord.x, v.coord.y);

    /* Top right */
    v.coord.x = get_coord_x(t, 1);
    v.coord.y = 0;
    v.color = get_color_rgba(255, 0, 0, 255);
    draw_point(t, v.color, v.coord.x, v.coord.y);

    /* Bottom right */
    v.coord.x = get_coord_x(t, 1);
    v.coord.y = get_coord_y(t, 1);
    v.color = get_color_rgba(0, 255, 0, 255);
    draw_point(t, v.color, v.coord.x, v.coord.y);

    /* Bottom left */
    v.coord.x = 0;
    v.coord.y = get_coord_y(t, 1);
    v.color = get_color_rgba(0, 0, 255, 255);
    draw_point(t, v.color, v.coord.x, v.coord.y);
    return;
}

void draw_test_center(t_tex* t)
{
    t_vert v;

    /* Top left */
    v.coord.x = get_coord_x(t, 0.5f);
    v.coord.y = get_coord_y(t, 0.5f);
    v.color = get_color_rgba(255, 255, 255, 255);
    draw_point(t, v.color, v.coord.x, v.coord.y);

    /* Top right */
    v.coord.x = get_coord_x(t, 0.5f) + 1;
    v.coord.y = get_coord_y(t, 0.5f);
    v.color = get_color_rgba(255, 0, 0, 255);
    draw_point(t, v.color, v.coord.x, v.coord.y);

    /* Bottom right */
    v.coord.x = get_coord_x(t, 0.5f) + 1;
    v.coord.y = get_coord_y(t, 0.5f) + 1;
    v.color = get_color_rgba(0, 255, 0, 255);
    draw_point(t, v.color, v.coord.x, v.coord.y);

    /* Bottom left */
    v.coord.x = get_coord_x(t, 0.5f);
    v.coord.y = get_coord_y(t, 0.5f) + 1;
    v.color = get_color_rgba(0, 0, 255, 255);
    draw_point(t, v.color, v.coord.x, v.coord.y);
    return;
}

void draw_test_lines(t_tex* t)
{
    t_vert v1;
    t_vert v2;

    v1.color = get_color_rgba(0, 255, 0, 255);
    v2.color = get_color_rgba(0, 255, 0, 255);

    v1.coord.x = get_coord_x(t, 0.5f) + 5;
    v1.coord.y = get_coord_y(t, 0.5f) + 5;
    v2.coord.x = v1.coord.x + 200;
    v2.coord.y = v1.coord.y + 100;
    draw_line(t, v1, v2);

    v1.color = get_color_rgba(255, 0, 0, 255);
    v2.color = get_color_rgba(255, 0, 0, 255);

    v1.coord.x -= normalize(v2.coord.x - v1.coord.x)*2;
    v1.coord.y -= normalize(v2.coord.y - v1.coord.y)*2;
    draw_point(t, v1.color, v1.coord.x, v1.coord.y);

    v2.coord.x -= normalize(v1.coord.x - v2.coord.x)*2;
    v2.coord.y -= normalize(v1.coord.y - v2.coord.y)*2;
    draw_point(t, v2.color, v2.coord.x, v2.coord.y);
    return;
}

void draw_test_rectangles(t_tex* t)
{
    t_vert v1;
    t_vert v2;
    t_ivec2 size;

    v1.coord.x = get_coord_x(t, 0.1f);
    v1.coord.y = get_coord_y(t, 0.33f);
    v1.color = get_color_rgba(255, 255, 255, 255);
    size.x = 100;
    size.y = 100;
    draw_rectangle_full(t, v1, size);

    v2.coord.x = get_coord_x(t, 0.1f) + 1;
    v2.coord.y = get_coord_y(t, 0.33f) + 1;
    v2.color = get_color_rgba(255, 0, 0, 255);
    draw_rectangle(t, v2, size);
    return;
}

void draw_test_circles(t_tex* t)
{
    int radius;
    t_color edge_color;
    t_vert v;
    
    radius = 100;
    edge_color = get_color_rgba(0, 0, 0, 255);
    v.coord.y = get_coord_y(t, 0.5f);

    v.color = get_color_rgba(255, 0, 0, 255);
    v.coord.x = get_coord_x(t, 0.25f);
    draw_circle_full_grad(t, v, radius, edge_color);

    v.color = get_color_rgba(0, 0, 255, 255);
    v.coord.x = get_coord_x(t, 0.5f);
    draw_circle_full_grad(t, v, radius, edge_color);

    v.color = get_color_rgba(0, 255, 0, 255);
    v.coord.x = get_coord_x(t, 0.75f);
    draw_circle_full_grad(t, v, radius, edge_color);
    return;
}

void draw_test_shapes(t_tex* t)
{
    /* Convex (triangle) */
    t_vert v[3];
    v[0].color = get_color_rgba(255, 0, 0, 255);
    v[1].color = get_color_rgba(0, 255, 0, 255);
    v[2].color = get_color_rgba(0, 0, 255, 255);

    v[0].coord.x = get_coord_x(t, 0.50f);
    v[0].coord.y = get_coord_y(t, 0.25f);
    v[1].coord.x = get_coord_x(t, 0.75f);
    v[1].coord.y = get_coord_y(t, 0.75f);
    v[2].coord.x = get_coord_x(t, 0.25f);
    v[2].coord.y = get_coord_y(t, 0.75f);

    /* Convex (rectangle) */
    /*
    t_vert v[4];
    v[0].color = get_color_rgba(255, 0, 0, 255);
    v[1].color = get_color_rgba(0, 255, 0, 255);
    v[2].color = get_color_rgba(0, 0, 255, 255);
    v[3].color = get_color_rgba(255, 255, 0, 255);

    v[0].coord.x = get_coord_x(t, 0.25f);
    v[0].coord.y = get_coord_y(t, 0.25f);
    v[1].coord.x = get_coord_x(t, 0.75f);
    v[1].coord.y = get_coord_y(t, 0.25f);
    v[2].coord.x = get_coord_x(t, 0.75f);
    v[2].coord.y = get_coord_y(t, 0.75f);
    v[3].coord.x = get_coord_x(t, 0.25f);
    v[3].coord.y = get_coord_y(t, 0.75f);
    */

    /* Concave (star) */
    /*
    t_vert v[10];
    v[0].color = get_color_rgba(255, 255, 255, 255);
    v[1].color = get_color_rgba(255, 0, 0, 255);
    v[2].color = get_color_rgba(0, 255, 0, 255);
    v[3].color = get_color_rgba(0, 0, 255, 255);
    v[4].color = get_color_rgba(255, 255, 0, 255);
    v[5].color = get_color_rgba(0, 255, 255, 255);
    v[6].color = get_color_rgba(255, 0, 255, 255);
    v[7].color = get_color_rgba(128, 128, 128, 255);
    v[8].color = get_color_rgba(255, 165, 0, 255);
    v[9].color = get_color_rgba(75, 0, 130, 255);

    // Top point (outer)
    v[0].coord.x = get_coord_x(t, 0.50f);
    v[0].coord.y = get_coord_y(t, 0.10f);
    // Right inner point
    v[1].coord.x = get_coord_x(t, 0.60f);
    v[1].coord.y = get_coord_y(t, 0.30f);
    // Right outer point
    v[2].coord.x = get_coord_x(t, 0.80f);
    v[2].coord.y = get_coord_y(t, 0.35f);
    // Bottom right inner point
    v[3].coord.x = get_coord_x(t, 0.65f);
    v[3].coord.y = get_coord_y(t, 0.55f);
    // Bottom right outer point
    v[4].coord.x = get_coord_x(t, 0.75f);
    v[4].coord.y = get_coord_y(t, 0.80f);
    // Bottom point (outer)
    v[5].coord.x = get_coord_x(t, 0.50f);
    v[5].coord.y = get_coord_y(t, 0.65f);
    // Bottom left outer point
    v[6].coord.x = get_coord_x(t, 0.25f);
    v[6].coord.y = get_coord_y(t, 0.80f);
    // Bottom left inner point
    v[7].coord.x = get_coord_x(t, 0.35f);
    v[7].coord.y = get_coord_y(t, 0.55f);
    // Left outer point
    v[8].coord.x = get_coord_x(t, 0.20f);
    v[8].coord.y = get_coord_y(t, 0.35f);
    // Left inner point
    v[9].coord.x = get_coord_x(t, 0.40f);
    v[9].coord.y = get_coord_y(t, 0.30f);
    */

    /* Concave and self-intercepting */
    /*
    t_vert v[5];
    v[0].color = get_color_rgba(255, 255, 255, 255);
    v[1].color = get_color_rgba(255, 0, 0, 255);
    v[2].color = get_color_rgba(0, 255, 0, 255);
    v[3].color = get_color_rgba(0, 0, 255, 255);
    v[4].color = get_color_rgba(255, 255, 0, 255);

    v[0].coord.x = get_coord_x(t, 0.25f);
    v[0].coord.y = get_coord_y(t, 0.25f);
    v[1].coord.x = get_coord_x(t, 0.50f);
    v[1].coord.y = get_coord_y(t, 0.75f);
    v[2].coord.x = get_coord_x(t, 0.30f);
    v[2].coord.y = get_coord_y(t, 0.44f);
    v[3].coord.x = get_coord_x(t, 0.75f);
    v[3].coord.y = get_coord_y(t, 0.25f);
    v[4].coord.x = get_coord_x(t, 0.50f);
    v[4].coord.y = get_coord_y(t, 0.10f);
    */

    draw_shape_full(t, v, sizeof(v)/sizeof(v[0]));
    return;
}

void draw_test_gradient(t_tex* t)
{
    t_vert v;

    v.color.b = 255 / 2;
    v.color.a = 255;
    v.coord.y = 0;
    while (v.coord.y < t->size.y)
    {
        v.coord.x = 0;
        while (v.coord.x < t->size.x)
        {
            v.color.r = 255 * v.coord.y / t->size.y;
            v.color.g = 255 * v.coord.x / t->size.x;
            draw_point(t, v.color, v.coord.x, v.coord.y);
            ++v.coord.x;
        }
        ++v.coord.y;
    }
    return;
}
