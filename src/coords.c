#include "cimmerian.h"

int get_coord_x(t_tex* t, double normalized)
{
    return (t->size.x - 1) * normalized;
}

int get_coord_y(t_tex* t, double normalized)
{
    return (t->size.y - 1) * normalized;
}

double get_coord_x_norm(t_tex* t, int coord)
{
    return (double)coord / (t->size.x - 1);
}

double get_coord_y_norm(t_tex* t, int coord)
{
    return (double)coord / (t->size.y - 1);
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

/* Hypotenuse, therefore Pythagorean theorem */
double get_distance(t_vec2 a, t_vec2 b)
{
    t_vec2 dir;

    dir = get_direction_double(a, b);
    return f_sqrt(dir.x * dir.x + dir.y * dir.y);
}
