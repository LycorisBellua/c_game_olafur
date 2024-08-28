#include "cimmerian.h"

void draw_image(t_frame* f, t_img* s)
{
    t_ivec2 i;

    i.y = 0;
    while (i.y < s->size.y)
    {
        i.x = 0;
        while (i.x < s->size.x)
        {
            draw_point(f, *((t_color*)s->buf + (i.y * s->size.x + i.x)), 
                i.x, i.y);
            ++i.x;
        }
        ++i.y;
    }
    return;
}
