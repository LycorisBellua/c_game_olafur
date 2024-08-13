#include "cimmerian.h"

void draw_point(t_tex* t, t_color c, int x, int y)
{
    int row;
    int col;
    t_color* buf;

    if (x < 0 || y < 0 || x >= t->size.x || y >= t->size.y)
        return;
    x *= t->thickness;
    y *= t->thickness;
    buf = (t_color*)t->buf + (y * t->real_size.x + x);
    c = get_alpha_blended_color(*buf, c);
    row = 0;
    while (row < t->thickness)
    {
        col = 0;
        while (col < t->thickness)
        {
            buf = (t_color*)t->buf + ((y + row) * t->real_size.x + (x + col));
            *buf = c;
            ++col;
        }
        ++row;
    }
    return;
}
