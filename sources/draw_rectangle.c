#include "../headers/draw_rectangle.h"
#include "../headers/maths.h"

static void draw_rectangle_empty(Texture* t, int x, int y, int width, int height);
static void draw_rectangle_full(Texture* t, int x, int y, int width, int height);
static void fix_rectangle_x(const int max_length, int* length, int* coord);
static void fix_rectangle_y(const int max_length, int* length, int* coord);

/* Coord is top left hand corner of shape */
void draw_rectangle(Texture* t, const int is_filled, int x, int y, int width, 
    int height)
{
    if (!is_filled)
        draw_rectangle_empty(t, x, y, width, height);
    else
        draw_rectangle_full(t, x, y, width, height);
    return;
}

static void draw_rectangle_empty(Texture* t, int x, int y, int width, int height)
{
    int i;
    int last_x, last_y, tmp_last_index;
    int x_out_of_bounds, y_out_of_bounds;

    /* Shape is invisible */
    if (width < 1 || height < 1)
        return;

    /* Shape is completely out of bounds */
    last_x = x+width-1;
    x_out_of_bounds = is_coord_out_of_bounds(t->width, x);
    if (last_x < 0 || x_out_of_bounds == 1)
        return;
    last_y = y-height+1;
    y_out_of_bounds = is_coord_out_of_bounds(t->height, y);
    if (last_y > t->height-1 || y_out_of_bounds == -1)
        return;

    tmp_last_index = MIN(last_x, t->width-1);

    /* Top line */
    if (!y_out_of_bounds)
    {
        for (i = MAX(0, x); i <= tmp_last_index; ++i)
            draw_point(t, i, y);
    }

    /* Bottom line */
    if (last_y >= 0)
    {
        for (i = MAX(0, x); i <= tmp_last_index; ++i)
            draw_point(t, i, last_y);
    }

    tmp_last_index = MAX(0, last_y);

    /* Left column */
    if (!x_out_of_bounds)
    {
        for (i = MIN(y-1, t->height-1); i > tmp_last_index; --i)
            draw_point(t, x, i);
    }

    /* Right column */
    if (last_x < t->width)
    {
        for (i = MIN(y-1, t->height-1); i > tmp_last_index; --i)
            draw_point(t, last_x, i);
    }
    return;
}

static void draw_rectangle_full(Texture* t, int x, int y, int width, int height)
{
    int i, j;

    fix_rectangle_x(t->width, &width, &x);
    if (!width)
        return;
    fix_rectangle_y(t->height, &height, &y);
    if (!height)
        return;

    for (j = y; j > y-height; --j)
    {
        for (i = x; i < x+width; ++i)
            draw_point(t, i, j);
    }
    return;
}

static void fix_rectangle_x(const int max_length, int* length, int* coord)
{
    int tmp;

    if (*length < 1)
    {
        *length = 0;
        return;
    }

    tmp = is_coord_out_of_bounds(max_length, *coord);

    /* To the right of the texture = shape is invisible */
    if (tmp == 1)
    {
        *length = 0;
        return;
    }

    /* To the left of the texture = shape might be visible */
    else if (tmp == -1)
    {
        *length += *coord;
        *coord = 0;
        if (*length < 1)
        {
            *length = 0;
            return;
        }
    }

    /* Coord is legal now, but clamp length if it bleeds out of bounds */
    tmp = max_length - *coord - *length;
    if (tmp < 0)
        *length += tmp;
    return;
}

static void fix_rectangle_y(const int max_length, int* length, int* coord)
{
    int tmp;

    if (*length < 1)
    {
        *length = 0;
        return;
    }

    tmp = is_coord_out_of_bounds(max_length, *coord);

    /* To the bottom of the texture = shape is invisible */
    if (tmp == -1)
    {
        *length = 0;
        return;
    }

    /* To the top of the texture = shape might be visible */
    else if (tmp == 1)
    {

        *length += max_length - *coord - 1;
        *coord = max_length - 1;
        if (*length < 1)
        {
            *length = 0;
            return;
        }
    }

    /* Coord is legal now, but clamp length if it bleeds out of bounds */
    tmp = *coord + 1 - *length;
    if (tmp < 0)
        *length += tmp;
    return;
}

