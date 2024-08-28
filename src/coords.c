#include "cimmerian.h"

void set_vec2(t_vec2* coord, double x, double y)
{
    coord->x = x;
    coord->y = y;
    return;
}

void set_ivec2(t_ivec2* coord, int x, int y)
{
    coord->x = x;
    coord->y = y;
    return;
}

int get_coord_x(t_frame* f, double normalized)
{
    return (f->size.x - 1) * normalized;
}

int get_coord_y(t_frame* f, double normalized)
{
    return (f->size.y - 1) * normalized;
}

double get_coord_x_norm(t_frame* f, int coord)
{
    return (double)coord / (f->size.x - 1);
}

double get_coord_y_norm(t_frame* f, int coord)
{
    return (double)coord / (f->size.y - 1);
}

t_ivec2 get_direction(t_ivec2 v1, t_ivec2 v2)
{
    t_ivec2 dir;

    dir.x = v1.x < 0 && v2.x < 0 ? abs(v1.x) + v2.x : v2.x - v1.x;
    dir.y = v1.y < 0 && v2.y < 0 ? abs(v1.y) + v2.y : v2.y - v1.y;
    return dir;
}

t_vec2 get_direction_double(t_vec2 v1, t_vec2 v2)
{
    t_vec2 dir;

    dir.x = v1.x < 0 && v2.x < 0 ? f_abs(v1.x) + v2.x : v2.x - v1.x;
    dir.y = v1.y < 0 && v2.y < 0 ? f_abs(v1.y) + v2.y : v2.y - v1.y;
    return dir;
}
