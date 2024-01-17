#include "../headers/draw.h"
#include "../headers/colors.h"

/* 
    The origin of the coordinate system is in the bottom left hand corner.
    The X axis range is [0, t->width] and the Y axis range is [0, t->height].
*/

static int is_coord_out_of_bounds(const int axis_length, const int coord);
static void clamp_straight_axis(const int max_length, int* length, int* coord);
static void draw_rectangle_empty(Texture* t, int x, int y, int width, 
    int height);
static void draw_rectangle_full(Texture* t, int x, int y, int width, 
    int height);

void draw_point(Texture* t, int x, int y)
{
    int row, col;

    /* TODO: Remove this check once the drawing functions are implemented */
    if (is_coord_out_of_bounds(t->width, x) 
        + is_coord_out_of_bounds(t->height, y))
    {
        printf("Forbidden coordinates: (%d,%d)\n", x, y);
        return;
    }
    /* ------------------------------------------------------------------ */

    x *= t->thickness;
    y *= t->thickness;

    for (row = 0; row < t->thickness; ++row)
    {
        for (col = 0; col < t->thickness; ++col)
            set_pixel_color(t->buffer + ((y+row) * t->real_width + x+col) * 4, 
                *color_default);
    }
    return;
}

void draw_line(Texture* t, int x1, int y1, int x2, int y2)
{
    /* TODO */
    int x, y;

    for (x = x1; x <= x2; ++x)
    {
        for (y = y1; y <= y2; ++y)
            draw_point(t, x, y);
    }
    return;
}

void draw_rectangle(Texture* t, const int is_filled, int x, int y, int width, 
    int height)
{
    if (!is_filled)
        draw_rectangle_empty(t, x, y, width, height);
    else
        draw_rectangle_full(t, x, y, width, height);
    return;
}

void draw_circle(Texture* t, const int filled_up, int x, int y, int radius)
{
    /* TODO */
    return;
}

/* Variadic arguments are coordinates: x1, y1, x2, y2... */
void draw_polygon(Texture* t, const int filled_up, ...)
{
    /*
        TODO
        - Make sure you can at least make a triangle with it.
        - Can you tell if it's convex before filling it up?
        - This function is not important. Discard?
    */
    return;
}

static int is_coord_out_of_bounds(const int axis_length, const int coord)
{
    const int val = axis_length - coord;

    /*
        If coord is `x`, it's out of bounds to the right of the texture.
        If coord is `y`, it's out of bounds to the top of the texture.
    */
    if (val < 1)
        return 1;

    /*
        If coord is `x`, it's out of bounds to the left of the texture.
        If coord is `y`, it's out of bounds to the bottom of the texture.
    */
    else if (val > axis_length)
        return -1;

    /* This coordinate is legal */
    else
        return 0;
}

static void clamp_straight_axis(const int max_length, int* length, int* coord)
{
    int coord_in_bounds = max_length - *coord;

    /*
        The shape is invisible.
        -----------------------------------------------------------------------
        A `length` inferior to 1 is no length. 

        A `coord_in_bounds` value inferior to 1 means that:
        - If coord is `x`, it's out of bounds to the right of the texture.
        - If coord is `y`, it's out of bounds to the top of the texture.
    */
    if (*length < 1 || coord_in_bounds < 1)
    {
        *length = 0;
        return;
    }

    /*
        The shape is either invisible or just needs to be troncated.
        -----------------------------------------------------------------------
        If coord is `x`, it's out of bounds to the left of the texture.
        If coord is `y`, it's out of bounds to the bottom of the texture.
    */
    else if (coord_in_bounds > max_length)
    {
        *length += *coord;
        *coord = 0;
        if (*length < 1)
        {
            *length = 0;
            return;
        }
    }

    /*
        The coord is now within bounds, but it doesn't mean the length doesn't 
        make the shape bleed out of bounds. If so, clamp the length.
    */
    coord_in_bounds -= *length;
    if (coord_in_bounds < 0)
        *length += coord_in_bounds;
    return;
}

static void draw_rectangle_empty(Texture* t, int x, int y, int width, 
    int height)
{
    int i;
    int last_x, last_y;
    int x_out_of_bounds, y_out_of_bounds;
    int last_x_out_of_bounds, last_y_out_of_bounds;

    /* Shape is invisible */
    if (width < 1 || height < 1)
        return;

    /* Shape is completely out of bounds on the X axis */
    last_x = x+width-1;
    if (last_x < 0)
        return;
    x_out_of_bounds = is_coord_out_of_bounds(t->width, x);
    if (x_out_of_bounds == 1)
        return;

    /* Shape is completely out of bounds on the Y axis */
    last_y = y+height-1;
    if (last_y < 0)
        return;
    y_out_of_bounds = is_coord_out_of_bounds(t->height, y);
    if (y_out_of_bounds == 1)
        return;

    /* Clamp X axis */
    if (x_out_of_bounds)
        x = 0;
    if ((last_x_out_of_bounds = !(last_x < t->width)))
        last_x = t->width-1;

    /* Clamp Y axis */
    if (y_out_of_bounds)
        y = 0;
    if ((last_y_out_of_bounds = !(last_y < t->height)))
        last_y = t->height-1;

    /* Draw bottom line */
    if (!y_out_of_bounds)
    {
        for (i = x; i <= last_x; ++i)
            draw_point(t, i, y);
    }

    /* Draw top line */
    if (!last_y_out_of_bounds)
    {
        for (i = x; i <= last_x; ++i)
            draw_point(t, i, last_y);
    }

    /* Draw left column */
    if (!x_out_of_bounds)
    {
        for (i = y; i <= last_y; ++i)
            draw_point(t, x, i);
    }

    /* Draw right column */
    if (!last_x_out_of_bounds)
    {
        for (i = y; i <= last_y; ++i)
            draw_point(t, last_x, i);
    }
    return;
}

static void draw_rectangle_full(Texture* t, int x, int y, int width, 
    int height)
{
    int i, j;

    clamp_straight_axis(t->width, &width, &x);
    if (!width)
        return;
    clamp_straight_axis(t->height, &height, &y);
    if (!height)
        return;

    for (j = y; j < y+height; ++j)
    {
        for (i = x; i < x+width; ++i)
            draw_point(t, i, j);
    }
    return;
}

