#include "cimmerian.h"

void draw_point(t_frame *f, t_color c, int x, int y)
{
    int row;
    int col;
    t_color *buf;

    if (x < 0 || y < 0 || x >= f->size.x || y >= f->size.y)
        return;
    x *= f->thickness;
    y *= f->thickness;
    buf = (t_color *)f->buf + (y * f->real_size.x + x);
    c = get_alpha_blended_color(*buf, c);
    row = 0;
    while (row < f->thickness)
    {
        col = 0;
        while (col < f->thickness)
        {
            buf = (t_color *)f->buf + ((y + row) * f->real_size.x + (x + col));
            *buf = c;
            ++col;
        }
        ++row;
    }
    return;
}
