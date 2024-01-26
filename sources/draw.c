#include "../headers/draw.h"
#include "../headers/draw_unsafe.h"
#include "../headers/maths.h"

static int clamp_straight_axis(int max_length, int* length, int* coord);
static int clamp_diagonal_line(Texture* t, Vector* p1, Vector* p2, Vector* dir);
static void draw_rectangle_full(Texture* t, Vertex v, int width, int height);
static void draw_rectangle_empty(Texture* t, Vertex v, int width, int height);
static void draw_circle_full(Texture* t, Vertex center, int radius);
static void draw_circle_empty(Texture* t, Vertex center, int radius);
static void draw_shape_full(Texture* t, Vertex arr[], int len);

int get_coord_x(Texture* t, float normalized)
{
    return (t->width-1) * normalized;
}

int get_coord_y(Texture* t, float normalized)
{
    return (t->height-1) * normalized;
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

void draw_point(Texture* t, GLubyte color, int x, int y)
{
    /*
        The coordinate parameters are individual ints and not a vector, so 
        that either a Vector or VectorF can be used. Of course, float coords 
        are converted into integers through being passed as arguments to this 
        function. Float coords are needed for drawing diagonal lines.

        As a bonus, being able to pass values directly into draw_point() 
        instead of declaring a vector variable is neat. May be useful.
    */

    int row;

    if (is_coord_out_of_bounds(t->width, x) 
        || is_coord_out_of_bounds(t->height, y))
        return;

    x *= t->thickness;
    y *= t->thickness;
    
    for (row = 0; row < t->thickness; ++row)
        memset(t->buffer + ((y+row) * t->real_width + x), color, t->thickness);
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
        draw_point_unsafe(t, v1.color, v1.coords.x, v1.coords.y);
    }
    else if (!dir.x)
    {
        if (is_coord_out_of_bounds(t->width, v1.coords.x))
            return;

        if (dir.y < 0)
            swap(&v1.coords.y, &v2.coords.y);
        v1.coords.y = CLAMP_MIN(v1.coords.y, 0);
        v2.coords.y = CLAMP_MAX(v2.coords.y, t->height-1);

        draw_line_vertical_unsafe(t, v1, v2.coords.y);
    }
    else if (!dir.y)
    {
        if (is_coord_out_of_bounds(t->height, v1.coords.y))
            return;

        if (dir.x < 0)
            swap(&v1.coords.x, &v2.coords.x);
        v1.coords.x = CLAMP_MIN(v1.coords.x, 0);
        v2.coords.x = CLAMP_MAX(v2.coords.x, t->width-1);

        draw_line_horizontal_unsafe(t, v1, v2.coords.x);
    }
    else
    {
        if (!clamp_diagonal_line(t, &v1.coords, &v2.coords, &dir))
            return;
        draw_line_diagonal_unsafe(t, v1, dir);
    }
    return;
}

void draw_line_horizontal(Texture* t, Vertex v, int last_x)
{
    if (is_coord_out_of_bounds(t->height, v.coords.y))
        return;

    while (v.coords.x <= last_x)
    {
        if (!is_coord_out_of_bounds(t->width, v.coords.x))
            draw_point_unsafe(t, v.color, v.coords.x, v.coords.y);
        ++v.coords.x;
    }
    return;
}

void draw_line_vertical(Texture* t, Vertex v, int last_y)
{
    if (is_coord_out_of_bounds(t->width, v.coords.x))
        return;

    while (v.coords.y <= last_y)
    {
        if (!is_coord_out_of_bounds(t->height, v.coords.y))
            draw_point_unsafe(t, v.color, v.coords.x, v.coords.y);
        ++v.coords.y;
    }
    return;
}

void draw_rectangle(Texture* t, int full, Vertex v, int width, int height)
{
    if (full)
        draw_rectangle_full(t, v, width, height);
    else
        draw_rectangle_empty(t, v, width, height);
    return;
}

void draw_circle(Texture* t, int full, Vertex v, int radius)
{
    Vector check_origin, check_opposite;

    if (radius < 1)
        return;

    check_origin.x = is_coord_out_of_bounds(t->width, v.coords.x - radius);
    check_origin.y = is_coord_out_of_bounds(t->height, v.coords.y - radius);
    check_opposite.x = is_coord_out_of_bounds(t->width, v.coords.x + radius);
    check_opposite.y = is_coord_out_of_bounds(t->height, v.coords.y + radius);

    if (!check_origin.x && !check_opposite.x 
        && !check_origin.y && !check_opposite.y)
    {
        if (full)
            draw_circle_full_unsafe(t, v, radius);
        else
            draw_circle_empty_unsafe(t, v, radius);
        return;
    }
    else if (check_origin.x && check_origin.x == check_opposite.x)
        return;
    else if (check_origin.y && check_origin.y == check_opposite.y)
        return;

    /*
        Circle at least partly visible.
        Add checks within the drawing functions.
    */

    if (full)
        draw_circle_full(t, v, radius);
    else
        draw_circle_empty(t, v, radius);
    return;
}

void draw_shape(Texture* t, int full, Vertex arr[], int len)
{
    int i;

    if (len < 3)
        return;

    if (full)
        draw_shape_full(t, arr, len);
    else
    {
        for (i = 0; i < len; ++i)
            draw_line(t, arr[i], arr[(i + 1) % len]);
    }
    return;
}

static int clamp_straight_axis(int max_length, int* length, int* coord)
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

    *dir = get_direction_unsafe(*p1, *p2);
    return 1;
}

static void draw_rectangle_full(Texture* t, Vertex v, int width, int height)
{
    if (!clamp_straight_axis(t->width, &width, &v.coords.x))
        return;
    if (!clamp_straight_axis(t->height, &height, &v.coords.y))
        return;

    draw_rectangle_full_unsafe(t, v, width, height);
    return;
}

static void draw_rectangle_empty(Texture* t, Vertex v, int width, int height)
{
    int tmp;
    Vector last_coords;
    Vector coords_out_of_bounds;
    Vector last_coords_out_of_bounds;

    if (width < 1 || height < 1)
        return;

    last_coords.x = v.coords.x + width-1;
    last_coords.y = v.coords.y + height-1;
    coords_out_of_bounds.x = is_coord_out_of_bounds(t->width, v.coords.x);
    coords_out_of_bounds.y = is_coord_out_of_bounds(t->height, v.coords.y);
    last_coords_out_of_bounds.x = is_coord_out_of_bounds(t->width, 
        last_coords.x);
    last_coords_out_of_bounds.y = is_coord_out_of_bounds(t->height, 
        last_coords.y);

    if (!coords_out_of_bounds.x && !coords_out_of_bounds.y 
        && !last_coords_out_of_bounds.x && !last_coords_out_of_bounds.y)
    {
        draw_rectangle_empty_unsafe(t, v, width, height);
        return;
    }

    if (coords_out_of_bounds.x)
    {
        /* Shape is completely out of bounds on the X axis */
        if (coords_out_of_bounds.x == last_coords_out_of_bounds.x)
            return;
        
        /* Clamp X axis */
        v.coords.x = 0;
        if (last_coords.x > t->width-1)
            last_coords.x = t->width-1;
    }

    if (coords_out_of_bounds.y)
    {
        /* Shape is completely out of bounds on the Y axis */
        if (coords_out_of_bounds.y == last_coords_out_of_bounds.y)
            return;

        /* Clamp Y axis */
        v.coords.y = 0;
        if (last_coords.y > t->height-1)
            last_coords.y = t->height-1;
    }

    /* Draw bottom line */
    if (!coords_out_of_bounds.y)
        draw_line_horizontal_unsafe(t, v, last_coords.x);

    /* Draw left column */
    if (!coords_out_of_bounds.x)
        draw_line_vertical_unsafe(t, v, last_coords.y);

    /* Draw top line */
    tmp = v.coords.y;
    if (!last_coords_out_of_bounds.y)
    {
        v.coords.y = last_coords.y;
        draw_line_horizontal_unsafe(t, v, last_coords.x);
    }

    /* Draw right column */
    if (!last_coords_out_of_bounds.x)
    {
        v.coords.x = last_coords.x;
        v.coords.y = tmp;
        draw_line_vertical_unsafe(t, v, last_coords.y);
    }
    return;
}

static void draw_circle_full(Texture* t, Vertex center, int radius)
{
    /* Midpoint Circle Drawing algorithm */
    int x = radius;
    int y = 0;
    int decision = 1 - radius;
    int last_coord;
    Vertex v;

    v.color = center.color;

    while (y <= x)
    {
        /**/
        v.coords.x = center.coords.x - x;
        last_coord = center.coords.x + x;

        v.coords.y = center.coords.y - y;
        draw_line_horizontal(t, v, last_coord);

        v.coords.y = center.coords.y + y;
        draw_line_horizontal(t, v, last_coord);

        /**/
        v.coords.x = center.coords.x - y;
        last_coord = center.coords.x + y;

        v.coords.y = center.coords.y - x;
        draw_line_horizontal(t, v, last_coord);

        v.coords.y = center.coords.y + x;
        draw_line_horizontal(t, v, last_coord);

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

static void draw_circle_empty(Texture* t, Vertex center, int radius)
{
    /* Midpoint Circle Drawing algorithm */
    int x = radius;
    int y = 0;
    int decision = 1 - radius;
    Vector point;

    while (y <= x)
    {
        /**/
        point.y = center.coords.y - y;

        point.x = center.coords.x - x;
        draw_point(t, center.color, point.x, point.y);

        point.x = center.coords.x + x;
        draw_point(t, center.color, point.x, point.y);

        /**/
        point.y = center.coords.y + y;

        point.x = center.coords.x - x;
        draw_point(t, center.color, point.x, point.y);

        point.x = center.coords.x + x;
        draw_point(t, center.color, point.x, point.y);

        /**/
        point.y = center.coords.y - x;

        point.x = center.coords.x - y;
        draw_point(t, center.color, point.x, point.y);

        point.x = center.coords.x + y;
        draw_point(t, center.color, point.x, point.y);

        /**/
        point.y = center.coords.y + x;

        point.x = center.coords.x - y;
        draw_point(t, center.color, point.x, point.y);

        point.x = center.coords.x + y;
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

static void draw_shape_full(Texture* t, Vertex arr[], int len)
{
    /* Scanline Fill algorithm */
    int i, j;
    int y, x1, x2, x_intersection;
    int ymin, ymax;
    Vertex v;
    v.color = arr[0].color;

    ymin = arr[0].coords.y;
    ymax = arr[0].coords.y;
    for (i = 1; i < len; ++i)
    {
        if (arr[i].coords.y < ymin)
            ymin = arr[i].coords.y;
        if (arr[i].coords.y > ymax)
            ymax = arr[i].coords.y;
    }

    for (y = ymin; y <= ymax; ++y)
    {
        x1 = t->width-1;
        x2 = 0;

        for (i = 0; i < len; ++i)
        {
            j = (i + 1) % len;
            if ((arr[i].coords.y <= y && arr[j].coords.y > y) 
                || (arr[j].coords.y <= y && arr[i].coords.y > y))
            {
                x_intersection = (arr[i].coords.x * (arr[j].coords.y - y) 
                    + arr[j].coords.x * (y - arr[i].coords.y)) 
                    / (arr[j].coords.y - arr[i].coords.y);

                if (x_intersection < x1)
                    x1 = x_intersection;
                if (x_intersection > x2)
                    x2 = x_intersection;
            }
        }

        if (x1 <= x2)
        {
            v.coords.x = x1;
            v.coords.y = y;
            draw_line_horizontal(t, v, x2);
        }
    }
    return;
}

/*
void draw_spinner(Texture* t, Vertex center, int radius)
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

