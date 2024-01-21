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
static void draw_line_horizontal_safe(Texture* t, Vertex v, int last_x);
static void draw_line_vertical(Texture* t, Vertex v, int last_y);
static void draw_line_diagonal(Texture* t, const Vertex v, const Vector dir);
static void draw_rectangle_full(Texture* t, Vertex v, int width, int height);
static void draw_rectangle_empty(Texture* t, Vertex v, int width, int height);
static void draw_circle_full(Texture* t, Vertex center, const int radius);
static void draw_circle_empty(Texture* t, Vertex center, const int radius);

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
    /*
        The coordinate parameters are individual ints and not a vector, so 
        that either a Vector or VectorF can be used. Of course, float coords 
        are converted into integers through being passed as arguments to this 
        function. Float coords are needed for drawing diagonal lines.

        As a bonus, being able to pass values directly into draw_point() 
        instead of declaring a vector variable is neat. May be useful.
    */

    int row, col;

    /* TODO: Remove this check once the drawing functions are implemented */
    if (is_coord_out_of_bounds(t->width, x) 
        || is_coord_out_of_bounds(t->height, y))
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
        draw_line_diagonal(t, v1, dir);
    }
    return;
}

void draw_rectangle(Texture* t, const int full, Vertex v, int width, 
    int height)
{
    if (full)
        draw_rectangle_full(t, v, width, height);
    else
        draw_rectangle_empty(t, v, width, height);
    return;
}

void draw_circle(Texture* t, const int full, Vertex v, const int radius)
{
    Vector check_origin, check_opposite;

    if (radius < 1)
        return;

    check_origin.x = is_coord_out_of_bounds(t->width, v.coords.x - radius);
    check_origin.y = is_coord_out_of_bounds(t->height, v.coords.y - radius);
    check_opposite.x = is_coord_out_of_bounds(t->width, v.coords.x + radius);
    check_opposite.y = is_coord_out_of_bounds(t->height, v.coords.y + radius);

    if (check_origin.x && check_origin.x == check_opposite.x)
        return;
    else if (check_origin.y && check_origin.y == check_opposite.y)
        return;

    /* Circle at least partly visible */
    if (full)
        draw_circle_full(t, v, radius);
    else
        draw_circle_empty(t, v, radius);
    return;
}

/* Variadic arguments are vertices: v1, v2... */
void draw_polygon(Texture* t, const int full, ...)
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

static void draw_line_horizontal_safe(Texture* t, Vertex v, int last_x)
{
    while (v.coords.x <= last_x)
    {
        if (!is_coord_out_of_bounds(t->width, v.coords.x))
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

static void draw_line_diagonal(Texture* t, const Vertex v, const Vector dir)
{
    int steps;
    VectorF coords, increment;

    steps = MAX(ABS(dir.x), ABS(dir.y));
    coords.x = v.coords.x;
    coords.y = v.coords.y;
    increment.x = dir.x / (float)steps;
    increment.y = dir.y / (float)steps;

    ++steps;

    while (steps-- > 0)
    {
        draw_point(t, v.color, coords.x, coords.y);
        coords.x += increment.x;
        coords.y += increment.y;
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

static void draw_circle_full(Texture* t, Vertex center, const int radius)
{
    int x = radius;
    int y = 0;
    int decision = 1 - radius;
    int last_coord;
    Vertex v;

    v.color = center.color;

    while (y <= x)
    {
        v.coords.x = center.coords.x - x;
        last_coord = center.coords.x + x;

        v.coords.y = center.coords.y - y;
        if (!is_coord_out_of_bounds(t->height, v.coords.y))
            draw_line_horizontal_safe(t, v, last_coord);

        v.coords.y = center.coords.y + y;
        if (!is_coord_out_of_bounds(t->height, v.coords.y))
            draw_line_horizontal_safe(t, v, last_coord);

        v.coords.x = center.coords.x - y;
        last_coord = center.coords.x + y;

        v.coords.y = center.coords.y - x;
        if (!is_coord_out_of_bounds(t->height, v.coords.y))
            draw_line_horizontal_safe(t, v, last_coord);

        v.coords.y = center.coords.y + x;
        if (!is_coord_out_of_bounds(t->height, v.coords.y))
            draw_line_horizontal_safe(t, v, last_coord);

        ++y;
        if (decision <= 0)
        {
            decision += 2 * y + 1;
        }
        else
        {
            --x;
            decision += 2 * (y - x) + 1;
        }
    }
    return;
}

static void draw_circle_empty(Texture* t, Vertex center, const int radius)
{
    int x = radius;
    int y = 0;
    int decision = 1 - radius;
    Vector point;

    while (y <= x)
    {
        point.x = center.coords.x + x;
        point.y = center.coords.y - y;
        if (!is_coord_out_of_bounds(t->width, point.x) 
            && !is_coord_out_of_bounds(t->height, point.y))
            draw_point(t, center.color, point.x, point.y);

        point.x = center.coords.x - x;
        point.y = center.coords.y - y;
        if (!is_coord_out_of_bounds(t->width, point.x) 
            && !is_coord_out_of_bounds(t->height, point.y))
            draw_point(t, center.color, point.x, point.y);

        point.x = center.coords.x + x;
        point.y = center.coords.y + y;
        if (!is_coord_out_of_bounds(t->width, point.x) 
            && !is_coord_out_of_bounds(t->height, point.y))
            draw_point(t, center.color, point.x, point.y);

        point.x = center.coords.x - x;
        point.y = center.coords.y + y;
        if (!is_coord_out_of_bounds(t->width, point.x) 
            && !is_coord_out_of_bounds(t->height, point.y))
            draw_point(t, center.color, point.x, point.y);

        point.x = center.coords.x + y;
        point.y = center.coords.y - x;
        if (!is_coord_out_of_bounds(t->width, point.x) 
            && !is_coord_out_of_bounds(t->height, point.y))
            draw_point(t, center.color, point.x, point.y);

        point.x = center.coords.x - y;
        point.y = center.coords.y - x;
        if (!is_coord_out_of_bounds(t->width, point.x) 
            && !is_coord_out_of_bounds(t->height, point.y))
            draw_point(t, center.color, point.x, point.y);

        point.x = center.coords.x + y;
        point.y = center.coords.y + x;
        if (!is_coord_out_of_bounds(t->width, point.x) 
            && !is_coord_out_of_bounds(t->height, point.y))
            draw_point(t, center.color, point.x, point.y);

        point.x = center.coords.x - y;
        point.y = center.coords.y + x;
        if (!is_coord_out_of_bounds(t->width, point.x) 
            && !is_coord_out_of_bounds(t->height, point.y))
            draw_point(t, center.color, point.x, point.y);

        ++y;
        if (decision <= 0)
        {
            decision += 2 * y + 1;
        }
        else
        {
            --x;
            decision += 2 * (y - x) + 1;
        }
    }
    return;
}

/*
void draw_spinner(Texture* t, Vertex center, const int radius)
{
    int x = radius;
    int y = 0;
    int decision = 1 - radius;

    while (y <= x)
    {
        if (y == 0 || x == 0 || y == x)
        {
            draw_point(t, center.color, center.coords.x + x, 
                                        center.coords.y - y);

            draw_point(t, center.color, center.coords.x - x, 
                                        center.coords.y - y);

            draw_point(t, center.color, center.coords.x + x, 
                                        center.coords.y + y);

            draw_point(t, center.color, center.coords.x - x, 
                                        center.coords.y + y);

            draw_point(t, center.color, center.coords.x + y, 
                                        center.coords.y - x);

            draw_point(t, center.color, center.coords.x - y, 
                                        center.coords.y - x);

            draw_point(t, center.color, center.coords.x + y, 
                                        center.coords.y + x);

            draw_point(t, center.color, center.coords.x - y, 
                                        center.coords.y + x);
        }

        ++y;
        if (decision <= 0)
        {
            decision += 2 * y + 1;
        }
        else
        {
            --x;
            decision += 2 * (y - x) + 1;
        }
    }
    return;
}
*/

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

void draw_test_circles(Texture* t)
{
    int radius = 100;
    Vertex v;
    v.color = colors[COLOR_BLUE];
    v.coords.x = get_coord_x(t, 0.7f);
    v.coords.y = get_coord_y(t, 0.4f);
    draw_circle(t, 1, v, radius);

    v.color = colors[COLOR_WHITE];
    draw_circle(t, 0, v, radius);
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

void draw_test_gradient_line(Texture* t)
{
    int i;
    const int dist = 200;
    const int half_dist = dist/2;
    float perc = 0;
    GLubyte first_color[4] = {0};
    GLubyte second_color[4] = {0};
    GLubyte color_change[4] = {0};
    Vector v;
    v.x = get_coord_x(t, 0.1f);
    v.y = get_coord_x(t, 0.4f);

    first_color[0] = colors[COLOR_RED][0];
    first_color[1] = colors[COLOR_RED][1];
    first_color[2] = colors[COLOR_RED][2];
    first_color[3] = colors[COLOR_RED][3];

    second_color[0] = colors[COLOR_GREEN][0];
    second_color[1] = colors[COLOR_GREEN][1];
    second_color[2] = colors[COLOR_GREEN][2];
    second_color[3] = colors[COLOR_GREEN][3];

    if (first_color[0] - second_color[0] 
        || first_color[1] - second_color[1] 
        || first_color[2] - second_color[2] 
        || first_color[3] - second_color[3])
    {
        perc = 100.0f / dist;

        color_change[0] = (second_color[0] - first_color[0]) / 50.0f * perc;
        color_change[1] = (second_color[1] - first_color[1]) / 50.0f * perc;
        color_change[2] = (second_color[2] - first_color[2]) / 50.0f * perc;
        color_change[3] = (second_color[3] - first_color[3]) / 50.0f * perc;
    }
    for (i = 0; i < half_dist; ++i)
    {
        draw_point(t, first_color, v.x, v.y);
        ++v.x;
        first_color[0] = CLAMP(first_color[0] + color_change[0], 0, 255);
        first_color[1] = CLAMP(first_color[1] + color_change[1], 0, 255);
        first_color[2] = CLAMP(first_color[2] + color_change[2], 0, 255);
        first_color[3] = CLAMP(first_color[3] + color_change[3], 0, 255);
    }

    if (perc > 0)
    {
        color_change[0] = (first_color[0] - second_color[0]) / 50.0f * perc;
        color_change[1] = (first_color[1] - second_color[1]) / 50.0f * perc;
        color_change[2] = (first_color[2] - second_color[2]) / 50.0f * perc;
        color_change[3] = (first_color[3] - second_color[3]) / 50.0f * perc;
    }
    for (; i < dist; ++i)
    {
        draw_point(t, first_color, v.x, v.y);
        ++v.x;
        first_color[0] = CLAMP(first_color[0] - color_change[0], 0, 255);
        first_color[1] = CLAMP(first_color[1] - color_change[1], 0, 255);
        first_color[2] = CLAMP(first_color[2] - color_change[2], 0, 255);
        first_color[3] = CLAMP(first_color[3] - color_change[3], 0, 255);
    }
    return;
}

