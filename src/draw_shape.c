#include "cimmerian.h"

static void draw_full_triangle(Texture* t, Vertex *v);
static void draw_blended_p(Texture* t, Vertex *v, Vector p, float inv_denom);

void draw_shape(Texture* t, Vertex arr[], int len)
{
    int i;

    i = 0;
    while (i < len)
    {
        draw_line(t, arr[i], arr[(i + 1) % len]);
        ++i;
    }
    return;
}

/* TODO: Use Delaunay triangulation for shapes of more than 3 vertices */
void draw_shape_full(Texture* t, Vertex arr[], int len)
{
    if (len < 3)
    {
        draw_shape(t, arr, len);
        return;
    }
    else if (len == 3)
        draw_full_triangle(t, arr);
    else
        draw_shape(t, arr, len);
    return;
}

/* Barycentric weights for color blending */
static void draw_full_triangle(Texture* t, Vertex *v)
{
    Vector min_coord;
    Vector max_coord;
    Vector p;
    float denom;
    float inv_denom;

    min_coord.x = min(min(v[0].coords.x, v[1].coords.x), v[2].coords.x);
    min_coord.y = min(min(v[0].coords.y, v[1].coords.y), v[2].coords.y);
    max_coord.x = max(max(v[0].coords.x, v[1].coords.x), v[2].coords.x);
    max_coord.y = max(max(v[0].coords.y, v[1].coords.y), v[2].coords.y);
    denom = (v[1].coords.y - v[2].coords.y) * (v[0].coords.x - v[2].coords.x)
        + (v[2].coords.x - v[1].coords.x) * (v[0].coords.y - v[2].coords.y);
    inv_denom = 1.0f / denom;
    p.y = min_coord.y;
    while (p.y <= max_coord.y)
    {
        p.x = min_coord.x;
        while (p.x <= max_coord.x)
        {
            draw_blended_p(t, v, p, inv_denom);
            ++p.x;
        }
        ++p.y;
    }
    return;
}

static void draw_blended_p(Texture* t, Vertex *v, Vector p, float inv_denom)
{
    Color color;
    float w[3];

    w[0] = ((v[1].coords.y - v[2].coords.y) * (p.x - v[2].coords.x) 
        + (v[2].coords.x - v[1].coords.x) * (p.y - v[2].coords.y)) * inv_denom;
    w[1] = ((v[2].coords.y - v[0].coords.y) * (p.x - v[2].coords.x) 
        + (v[0].coords.x - v[2].coords.x) * (p.y - v[2].coords.y)) * inv_denom;
    w[2] = 1.0f - w[0] - w[1];
    if (w[0] < 0 || w[1] < 0 || w[2] < 0)
        return;
    color.r = v[0].color.r * w[0] + v[1].color.r * w[1] + v[2].color.r * w[2];
    color.g = v[0].color.g * w[0] + v[1].color.g * w[1] + v[2].color.g * w[2];
    color.b = v[0].color.b * w[0] + v[1].color.b * w[1] + v[2].color.b * w[2];
    color.a = v[0].color.a * w[0] + v[1].color.a * w[1] + v[2].color.a * w[2];
    draw_point(t, color, p.x, p.y);
    return;
}
