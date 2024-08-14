#include "cimmerian.h"

void draw_sprite(t_tex* t, t_spr* s)
{
    t_uivec2 i;

    i.y = 0;
    while (i.y < s->size.y)
    {
        i.x = 0;
        while (i.x < s->size.x)
        {
            draw_point(t, *((t_color*)s->buf + (i.y * s->size.x + i.x)), 
                i.x, i.y);
            ++i.x;
        }
        ++i.y;
    }
    return;
}
