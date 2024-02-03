#include "../headers/coords.h"
#include "../headers/maths.h"

int get_coord_x(Texture* t, float normalized)
{
    return (t->width-1) * normalized;
}

int get_coord_y(Texture* t, float normalized)
{
    return (t->height-1) * normalized;
}

float get_coord_x_norm(Texture* t, int coord)
{
    return (float)coord / (t->width-1);
}

float get_coord_y_norm(Texture* t, int coord)
{
    return (float)coord / (t->height-1);
}

int is_coord_out_of_bounds(int axis_length, int coord)
{
    /*
        If coord is `x`, it's out of bounds to the left of the texture.
        If coord is `y`, it's out of bounds to the bottom of the texture.
    */
    if (coord < 0)
        return -1;

    /*
        If coord is `x`, it's out of bounds to the right of the texture.
        If coord is `y`, it's out of bounds to the top of the texture.
    */
    else if (coord >= axis_length)
        return 1;

    /* This coordinate is legal */
    else
        return 0;
}

Vector get_direction(Vector v1, Vector v2)
{
    Vector dir;

    dir.x = v1.x < 0 && v2.x < 0 ? ABS(v1.x) + v2.x : v2.x - v1.x;
    dir.y = v1.y < 0 && v2.y < 0 ? ABS(v1.y) + v2.y : v2.y - v1.y;

    return dir;
}

Vector get_direction_unsafe(Vector v1, Vector v2)
{
    Vector dir;

    dir.x = v2.x - v1.x;
    dir.y = v2.y - v1.y;

    return dir;
}

VectorF get_direction_float(VectorF v1, VectorF v2)
{
    VectorF dir;

    dir.x = v1.x < 0 && v2.x < 0 ? ABS(v1.x) + v2.x : v2.x - v1.x;
    dir.y = v1.y < 0 && v2.y < 0 ? ABS(v1.y) + v2.y : v2.y - v1.y;

    return dir;
}

float get_distance(const VectorF a, const VectorF b)
{
    /* Hypotenuse, therefore Pythagorean theorem */
    VectorF dir = get_direction_float(a, b);
    return f_sqrt(dir.x * dir.x + dir.y * dir.y);
}

