#include "cimmerian.h"

void draw_point(Texture* t, Color c, int x, int y)
{
    int row;
    int col;

    if (is_coord_out_of_bounds(t->width, x) 
        || is_coord_out_of_bounds(t->height, y))
        return;
    x *= t->thickness;
    y *= t->thickness;
    c = get_alpha_blended_color(*((Color*)t->buffer + (y * t->real_width + x)),
        c);
    row = 0;
    while (row < t->thickness)
    {
        col = 0;
        while (col < t->thickness)
        {
            *((Color*)t->buffer + ((y + row) * t->real_width + (x + col))) = c;
            ++col;
        }
        ++row;
    }
    return;
}
