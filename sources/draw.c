#include "../headers/draw.h"
#include "../headers/colors.h"
#include "../headers/maths.h"

/* 
    The origin of the coordinate system is in the bottom left hand corner.
    The X axis range is [0, t->width] and the Y axis range is [0, t->height].
*/

static Vector get_direction(const Vector v1, const Vector v2);
static int is_coord_out_of_bounds(const int axis_length, const int coord);
static int clamp_straight_axis(const int max_length, int* length, int* coord);
static int clamp_diagonal_line(Texture* t, Vector* p1, Vector* p2, Vector* dir);
static void draw_line_horizontal(Texture* t, Vertex v, int last_x);
static void draw_line_vertical(Texture* t, Vertex v, int last_y);
static void draw_line_diagonal(Texture* t, Vertex v1, Vertex v2, Vector dir);
static void draw_rectangle_empty(Texture* t, Vertex v, int width, int height);
static void draw_rectangle_full(Texture* t, Vertex v, int width, int height);

int get_coord_x(Texture* t, const float normalized)
{
    return (t->width-1) * normalized;
}

int get_coord_y(Texture* t, const float normalized)
{
    return (t->height-1) * normalized;
}

void draw_point(Texture* t, GLubyte* color, int x, int y)
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
    
    /* First row: Place each pixel one by one */
    for (col = 0; col < t->thickness; ++col)
        memcpy(t->buffer + (y * t->real_width + x+col) * 4, color, 4);

    /* Use the first row to write the other ones */
    for (row = 1; row < t->thickness; ++row)
    {
        memcpy(t->buffer + ((y+row) * t->real_width + x) * 4, 
            t->buffer + (y * t->real_width + x) * 4, 
            4 * t->thickness);
    }
    return;
}

void draw_line(Texture* t, Vertex v1, Vertex v2)
{
    Vector dir = get_direction(v1.coords, v2.coords);

    if (!dir.x && !dir.y)
    {
        if (is_coord_out_of_bounds(t->width, v1.coords.x) 
                || is_coord_out_of_bounds(t->height, v1.coords.y))
            return;
        draw_point(t, v1.color, v1.coords.x, v1.coords.y);
    }
    else if (!dir.x)
    {
        if (is_coord_out_of_bounds(t->width, v1.coords.x))
            return;

        if (dir.y < 0)
            swap(&v1.coords.y, &v2.coords.y);
        v1.coords.y = CLAMP_MIN(v1.coords.y, 0);
        v2.coords.y = CLAMP_MAX(v2.coords.y, t->height-1);

        draw_line_vertical(t, v1, v2.coords.y);
    }
    else if (!dir.y)
    {
        if (is_coord_out_of_bounds(t->height, v1.coords.y))
            return;

        if (dir.x < 0)
            swap(&v1.coords.x, &v2.coords.x);
        v1.coords.x = CLAMP_MIN(v1.coords.x, 0);
        v2.coords.x = CLAMP_MAX(v2.coords.x, t->width-1);

        draw_line_horizontal(t, v1, v2.coords.x);
    }
    else
    {
        if (!clamp_diagonal_line(t, &v1.coords, &v2.coords, &dir))
            return;
        draw_line_diagonal(t, v1, v2, dir);
    }
    return;
}

void draw_rectangle(Texture* t, const int is_filled, Vertex v, int width, 
    int height)
{
    if (!is_filled)
        draw_rectangle_empty(t, v, width, height);
    else
        draw_rectangle_full(t, v, width, height);
    return;
}

void draw_circle(Texture* t, const int filled_up, Vertex v, int radius)
{
    /* TODO */
    return;
}

/* Variadic arguments are vertices: v1, v2... */
void draw_polygon(Texture* t, const int filled_up, ...)
{
    /* TODO */
    return;
}

static Vector get_direction(const Vector v1, const Vector v2)
{
    Vector dir;

    dir.x = v1.x < 0 && v2.x < 0 ? ABS(v1.x) + v2.x : v2.x - v1.x;
    dir.y = v1.y < 0 && v2.y < 0 ? ABS(v1.y) + v2.y : v2.y - v1.y;

    return dir;
}

static int is_coord_out_of_bounds(const int axis_length, const int coord)
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

static int clamp_straight_axis(const int max_length, int* length, int* coord)
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
        return 0;

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
            return 0;
    }

    /*
        The coord is now within bounds, but it doesn't mean the length doesn't 
        make the shape bleed out of bounds. If so, clamp the length.
    */
    coord_in_bounds -= *length;
    if (coord_in_bounds < 0)
        *length += coord_in_bounds;
    return 1;
}

static int clamp_diagonal_line(Texture* t, Vector* p1, Vector* p2, Vector* dir)
{
    int tmp;
    float slope;
    VectorF new_p1, new_p2;
    Vector p1_out_of_bounds, p2_out_of_bounds;

    p1_out_of_bounds.x = is_coord_out_of_bounds(t->width, p1->x);
    p1_out_of_bounds.y = is_coord_out_of_bounds(t->height, p1->y);
    p2_out_of_bounds.x = is_coord_out_of_bounds(t->width, p2->x);
    p2_out_of_bounds.y = is_coord_out_of_bounds(t->height, p2->y);

    /* Line completely out of bounds */
    if (ABS(p1_out_of_bounds.x + p2_out_of_bounds.x) == 2 
            || ABS(p1_out_of_bounds.y + p2_out_of_bounds.y) == 2)
        return 0;

    new_p1.x = p1->x;
    new_p1.y = p1->y;
    new_p2.x = p2->x;
    new_p2.y = p2->y;
    slope = dir->y / (float)dir->x;

    if (p1_out_of_bounds.x)
    {
        tmp = p1_out_of_bounds.x == -1 ? 0 : t->width-1;
        new_p1.y = p1->y - (p1->x - tmp) * slope;
        new_p1.x = tmp;
    }

    if (p2_out_of_bounds.x)
    {
        tmp = p2_out_of_bounds.x == -1 ? 0 : t->width-1;
        new_p2.y = p1->y + (tmp - p1->x) * slope;
        new_p2.x = tmp;
    }

    if (p1_out_of_bounds.y)
    {
        tmp = p1_out_of_bounds.y == -1 ? 0 : t->height-1;
        new_p1.x = p1->x - (p1->y - tmp) / slope;
        new_p1.y = tmp;
    }

    if (p2_out_of_bounds.y)
    {
        tmp = p2_out_of_bounds.y == -1 ? 0 : t->height-1;
        new_p2.x = p1->x + (tmp - p1->y) / slope;
        new_p2.y = tmp;
    }

    p1->x = CLAMP(new_p1.x, 0, t->width-1);
    p1->y = CLAMP(new_p1.y, 0, t->height-1);
    p2->x = CLAMP(new_p2.x, 0, t->width-1);
    p2->y = CLAMP(new_p2.y, 0, t->height-1);

    *dir = get_direction(*p1, *p2);
    return 1;
}

static void draw_line_horizontal(Texture* t, Vertex v, int last_x)
{
    while (v.coords.x <= last_x)
    {
        draw_point(t, v.color, v.coords.x, v.coords.y);
        ++v.coords.x;
    }
    return;
}

static void draw_line_vertical(Texture* t, Vertex v, int last_y)
{
    while (v.coords.y <= last_y)
    {
        draw_point(t, v.color, v.coords.x, v.coords.y);
        ++v.coords.y;
    }
    return;
}

static void draw_line_diagonal(Texture* t, Vertex v1, Vertex v2, Vector dir)
{
    int steps;
    VectorF coords, increment;

    steps = MAX(ABS(dir.x), ABS(dir.y));
    coords.x = v1.coords.x;
    coords.y = v1.coords.y;
    increment.x = dir.x / (float)steps;
    increment.y = dir.y / (float)steps;

    ++steps;

    while (steps-- > 0)
    {
        draw_point(t, v1.color, coords.x, coords.y);
        coords.x += increment.x;
        coords.y += increment.y;
    }
    return;
}

static void draw_rectangle_empty(Texture* t, Vertex v, int width, int height)
{
    Vector last_coords;
    Vector coords_out_of_bounds;
    Vector last_coords_out_of_bounds;

    /* Shape is invisible */
    if (width < 1 || height < 1)
        return;

    /* Shape is completely out of bounds on the X axis */
    last_coords.x = v.coords.x + width-1;
    if (last_coords.x < 0)
        return;
    coords_out_of_bounds.x = is_coord_out_of_bounds(t->width, v.coords.x);
    if (coords_out_of_bounds.x == 1)
        return;

    /* Shape is completely out of bounds on the Y axis */
    last_coords.y = v.coords.y + height-1;
    if (last_coords.y < 0)
        return;
    coords_out_of_bounds.y = is_coord_out_of_bounds(t->height, v.coords.y);
    if (coords_out_of_bounds.y == 1)
        return;

    /* Clamp X axis */
    if (coords_out_of_bounds.x)
        v.coords.x = 0;
    if ((last_coords_out_of_bounds.x = !(last_coords.x < t->width)))
        last_coords.x = t->width-1;

    /* Clamp Y axis */
    if (coords_out_of_bounds.y)
        v.coords.y = 0;
    if ((last_coords_out_of_bounds.y = !(last_coords.y < t->height)))
        last_coords.y = t->height-1;

    /* Draw bottom line */
    if (!coords_out_of_bounds.y)
        draw_line_horizontal(t, v, last_coords.x);

    /* Draw left column */
    if (!coords_out_of_bounds.x)
        draw_line_vertical(t, v, last_coords.y);

    /* Draw top line */
    if (!last_coords_out_of_bounds.y)
    {
        last_coords_out_of_bounds.y = v.coords.y;
        v.coords.y = last_coords.y;
        draw_line_horizontal(t, v, last_coords.x);
    }

    /* Draw right column */
    if (!last_coords_out_of_bounds.x)
    {
        v.coords.x = last_coords.x;
        v.coords.y = last_coords_out_of_bounds.y;
        draw_line_vertical(t, v, last_coords.y);
    }
    return;
}

static void draw_rectangle_full(Texture* t, Vertex v, int width, int height)
{
    Vector last_coords;

    if (!clamp_straight_axis(t->width, &width, &v.coords.x))
        return;
    if (!clamp_straight_axis(t->height, &height, &v.coords.y))
        return;

    last_coords.x = v.coords.x + width-1;
    last_coords.y = v.coords.y + height-1;

    while (v.coords.y <= last_coords.y)
    {
        draw_line_horizontal(t, v, last_coords.x);
        ++v.coords.y;
    }
    return;
}

/* Test functions ---------------------------------------------------------- */

void draw_test_corners(Texture* t)
{
    Vertex v;

    /* Bottom left */
    v.coords.x = 0;
    v.coords.y = 0;
    v.color = colors[COLOR_WHITE];
    draw_point(t, v.color, v.coords.x, v.coords.y);

    /* Top left */
    v.coords.x = 0;
    v.coords.y = get_coord_y(t, 1);
    v.color = colors[COLOR_RED];
    draw_point(t, v.color, v.coords.x, v.coords.y);

    /* Top right */
    v.coords.x = get_coord_x(t, 1);
    v.coords.y = get_coord_y(t, 1);
    v.color = colors[COLOR_GREEN];
    draw_point(t, v.color, v.coords.x, v.coords.y);

    /* Bottom right */
    v.coords.x = get_coord_x(t, 1);
    v.coords.y = 0;
    v.color = colors[COLOR_BLUE];
    draw_point(t, v.color, v.coords.x, v.coords.y);
    return;
}

void draw_test_center(Texture* t)
{
    Vertex v;

    /* Bottom left */
    v.coords.x = get_coord_x(t, 0.5f);
    v.coords.y = get_coord_y(t, 0.5f);
    v.color = colors[COLOR_WHITE];
    draw_point(t, v.color, v.coords.x, v.coords.y);

    /* Top left */
    v.coords.x = get_coord_x(t, 0.5f);
    v.coords.y = get_coord_y(t, 0.5f) + 1;
    v.color = colors[COLOR_RED];
    draw_point(t, v.color, v.coords.x, v.coords.y);

    /* Top right */
    v.coords.x = get_coord_x(t, 0.5f) + 1;
    v.coords.y = get_coord_y(t, 0.5f) + 1;
    v.color = colors[COLOR_GREEN];
    draw_point(t, v.color, v.coords.x, v.coords.y);

    /* Bottom right */
    v.coords.x = get_coord_x(t, 0.5f) + 1;
    v.coords.y = get_coord_y(t, 0.5f);
    v.color = colors[COLOR_BLUE];
    draw_point(t, v.color, v.coords.x, v.coords.y);
    return;
}

void draw_test_lines(Texture* t)
{
    Vertex v1, v2;

    v1.color = colors[COLOR_GREEN];
    v2.color = colors[COLOR_GREEN];

    v1.coords.x = get_coord_x(t, 0.5f) + 5;
    v1.coords.y = get_coord_y(t, 0.5f) + 5;
    v2.coords.x = v1.coords.x + 200;
    v2.coords.y = v1.coords.y + 100;
    draw_line(t, v1, v2);

    v1.color = colors[COLOR_RED];
    v2.color = colors[COLOR_RED];

    v1.coords.x -= NORMALIZE(v2.coords.x - v1.coords.x)*2;
    v1.coords.y -= NORMALIZE(v2.coords.y - v1.coords.y)*2;
    if (!is_coord_out_of_bounds(t->width, v1.coords.x) 
        && !is_coord_out_of_bounds(t->height, v1.coords.y))
        draw_point(t, v1.color, v1.coords.x, v1.coords.y);

    v2.coords.x -= NORMALIZE(v1.coords.x - v2.coords.x)*2;
    v2.coords.y -= NORMALIZE(v1.coords.y - v2.coords.y)*2;
    if (!is_coord_out_of_bounds(t->width, v2.coords.x) 
        && !is_coord_out_of_bounds(t->height, v2.coords.y))
        draw_point(t, v2.color, v2.coords.x, v2.coords.y);
    return;
}

void draw_test_rectangles(Texture* t)
{
    Vertex v1, v2;

    v1.coords.x = get_coord_x(t, 0.1f) - 1;
    v1.coords.y = get_coord_y(t, 0.33f) - 1;
    v1.color = colors[COLOR_WHITE];
    draw_rectangle(t, 1, v1, 100, 100);

    v2.coords.x = get_coord_x(t, 0.1f);
    v2.coords.y = get_coord_y(t, 0.33f) - 2;
    v2.color = colors[COLOR_RED];
    draw_rectangle(t, 0, v2, 100, 100);

    return;
}

void draw_test_gradient(Texture* t)
{
    Vertex v;
    GLubyte color[4];
    color[2] = 255/2;
    color[3] = 255;
    v.color = color;

    for (v.coords.y = 0; v.coords.y < t->height; ++v.coords.y)
    {
        for (v.coords.x = 0; v.coords.x < t->width; ++v.coords.x)
        {
            color[0] = v.coords.x * 255 / t->width;
            color[1] = v.coords.y * 255 / t->height;
            draw_point(t, v.color, v.coords.x, v.coords.y);
        }
    }
    return;
}

