#include "cimmerian.h"

void draw_rectangle(Texture* t, Vertex v, Vector size)
{
    Vertex v2;
    Vertex v3;
    Vertex v4;

    v2.color = v.color;
    v3.color = v.color;
    v4.color = v.color;
    v2.coords.x = v.coords.x + size.x - 1;
    v2.coords.y = v.coords.y;
    v3.coords.x = v.coords.x;
    v3.coords.y = v.coords.y + size.y - 1;
    v4.coords.x = v.coords.x + size.x - 1;
    v4.coords.y = v.coords.y + size.y - 1;
    draw_line(t, v, v2);
    ++v2.coords.y;
    draw_line(t, v2, v4);
    --v4.coords.x;
    draw_line(t, v3, v4);
    ++v.coords.y;
    --v3.coords.y;
    draw_line(t, v, v3);
    return;
}

void draw_rectangle_full(Texture* t, Vertex v, Vector size)
{
    int target_y;
    Vertex v2;

    target_y = v.coords.y + size.y - 1;
    v2.coords.x = v.coords.x + size.x - 1;
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
