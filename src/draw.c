#include "cimmerian.h"

static void draw_rectangle_full(Texture* t, Vertex v, int width, int height);
static void draw_rectangle_empty(Texture* t, Vertex v, int width, int height);
static void draw_circle_full(Texture* t, Vertex center, int radius);
static void draw_circle_empty(Texture* t, Vertex center, int radius);
static void draw_shape_full(Texture* t, Vertex arr[], int len);

/*
    The coordinate parameters are individual ints and not a vector, so 
    that either a Vector or VectorF can be used. Of course, double coords 
    are converted into integers through being passed as arguments to this 
    function. double coords are needed for drawing diagonal lines.

    As a bonus, being able to pass values directly into draw_point() 
    instead of declaring a vector variable is neat. May be useful.
*/
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

void draw_line(Texture* t, Vertex v1, Vertex v2)
{
    Vector dir;
    int steps, total_steps;
    VectorF coords, increment;
    Color color;
    double t_factor;

    dir = get_direction(v1.coords, v2.coords);
    steps = max(abs(dir.x), abs(dir.y));
    coords.x = v1.coords.x;
    coords.y = v1.coords.y;
    increment.x = dir.x / (double)steps;
    increment.y = dir.y / (double)steps;
    ++steps;
    total_steps = steps;
    while (steps-- > 0)
    {
        t_factor = (double)(total_steps - steps - 1) / (total_steps - 1);
        color.r = (GLubyte)(v1.color.r * (1.0 - t_factor) + v2.color.r * t_factor);
        color.g = (GLubyte)(v1.color.g * (1.0 - t_factor) + v2.color.g * t_factor);
        color.b = (GLubyte)(v1.color.b * (1.0 - t_factor) + v2.color.b * t_factor);
        color.a = (GLubyte)(v1.color.a * (1.0 - t_factor) + v2.color.a * t_factor);
        draw_point(t, color, coords.x, coords.y);
        coords.x += increment.x;
        coords.y += increment.y;
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
        i = 0;
        while (i < len)
        {
            draw_line(t, arr[i], arr[(i + 1) % len]);
            ++i;
        }
    }
    return;
}

static void draw_rectangle_full(Texture* t, Vertex v, int width, int height)
{
    int target_y;
    Vertex v2;

    target_y = v.coords.y + height-1;
    v2.coords.x = v.coords.x + width-1;
    v2.coords.y = v.coords.y;
    v2.color = v.color;
    while (v.coords.y <= target_y)
    {
        draw_line(t, v, v2);
        ++v.coords.y;
        ++v2.coords.y;
    }
    return;
}

static void draw_rectangle_empty(Texture* t, Vertex v, int width, int height)
{
    Vertex v2;
    Vertex v3;
    Vertex v4;

    v2.color = v.color;
    v3.color = v.color;
    v4.color = v.color;

    v2.coords.x = v.coords.x + width-1;
    v2.coords.y = v.coords.y;
    v3.coords.x = v.coords.x;
    v3.coords.y = v.coords.y + height-1;
    v4.coords.x = v.coords.x + width-1;
    v4.coords.y = v.coords.y + height-1;

    /* Top */
    draw_line(t, v, v2);

    /* Right */
    ++v2.coords.y;
    draw_line(t, v2, v4);

    /* Bottom */
    --v4.coords.x;
    draw_line(t, v3, v4);

    /* Left */
    ++v.coords.y;
    --v3.coords.y;
    draw_line(t, v, v3);
    return;
}

/* Midpoint Circle Drawing algorithm */
static void draw_circle_full(Texture* t, Vertex center, int radius)
{
    int x = radius;
    int y = 0;
    int decision = 1 - radius;
    Vertex v1;
    Vertex v2;
    v1.color = center.color;
    v2.color = v1.color;

    while (y <= x)
    {
        v1.coords.x = center.coords.x - x;
        v2.coords.x = center.coords.x + x;

        v1.coords.y = center.coords.y - y;
        v2.coords.y = v1.coords.y;
        draw_line(t, v1, v2);

        v1.coords.y = center.coords.y + y;
        v2.coords.y = v1.coords.y;
        draw_line(t, v1, v2);

        v1.coords.x = center.coords.x - y;
        v2.coords.x = center.coords.x + y;

        v1.coords.y = center.coords.y - x;
        v2.coords.y = v1.coords.y;
        draw_line(t, v1, v2);

        v1.coords.y = center.coords.y + x;
        v2.coords.y = v1.coords.y;
        draw_line(t, v1, v2);

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

/* Midpoint Circle Drawing algorithm */
static void draw_circle_empty(Texture* t, Vertex center, int radius)
{
    int x = radius;
    int y = 0;
    int decision = 1 - radius;
    Vector point;

    while (y <= x)
    {
        point.y = center.coords.y - y;

        point.x = center.coords.x - x;
        draw_point(t, center.color, point.x, point.y);

        point.x = center.coords.x + x;
        draw_point(t, center.color, point.x, point.y);

        point.y = center.coords.y + y;

        point.x = center.coords.x - x;
        draw_point(t, center.color, point.x, point.y);

        point.x = center.coords.x + x;
        draw_point(t, center.color, point.x, point.y);

        point.y = center.coords.y - x;

        point.x = center.coords.x - y;
        draw_point(t, center.color, point.x, point.y);

        point.x = center.coords.x + y;
        draw_point(t, center.color, point.x, point.y);

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

/* Scanline Fill algorithm */
/*
static void draw_shape_full(Texture* t, Vertex arr[], int len)
{
    int i, j;
    int y, x1, x2, x_intersection;
    int ymin, ymax;
    Vertex v1;
    Vertex v2;
    v1.color = arr[0].color;
    v2.color = v1.color;

    ymin = arr[0].coords.y;
    ymax = arr[0].coords.y;
    i = 1;
    while (i < len)
    {
        if (arr[i].coords.y < ymin)
            ymin = arr[i].coords.y;
        if (arr[i].coords.y > ymax)
            ymax = arr[i].coords.y;
        ++i;
    }

    y = ymin;
    while (y <= ymax)
    {
        x1 = t->width-1;
        x2 = 0;

        i = 0;
        while (i < len)
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
            ++i;
        }

        if (x1 <= x2)
        {
            v1.coords.x = x1;
            v2.coords.x = x2;
            v1.coords.y = y;
            v2.coords.y = y;
            draw_line(t, v1, v2);
        }
        ++y;
    }
    return;
}
*/
/*
Color interpolate_color_barycentric(const Vertex* v0, const Vertex* v1, const Vertex* v2, float w0, float w1, float w2)
{
    Color color;
    color.r = (GLubyte)(v0->color.r * w0 + v1->color.r * w1 + v2->color.r * w2);
    color.g = (GLubyte)(v0->color.g * w0 + v1->color.g * w1 + v2->color.g * w2);
    color.b = (GLubyte)(v0->color.b * w0 + v1->color.b * w1 + v2->color.b * w2);
    color.a = (GLubyte)(v0->color.a * w0 + v1->color.a * w1 + v2->color.a * w2);
    return color;
}

void calculate_barycentric_weights(Vector p, const Vector* p0, const Vector* p1, const Vector* p2, float* w0, float* w1, float* w2)
{
    int denominator = (p1->y - p2->y) * (p0->x - p2->x) + (p2->x - p1->x) * (p0->y - p2->y);
    *w0 = (float)((p1->y - p2->y) * (p.x - p2->x) + (p2->x - p1->x) * (p.y - p2->y)) / denominator;
    *w1 = (float)((p2->y - p0->y) * (p.x - p2->x) + (p0->x - p2->x) * (p.y - p2->y)) / denominator;
    *w2 = 1.0f - *w0 - *w1;
}

static void draw_shape_full(Texture* t, Vertex arr[], int len)
{
    int i, j, x;
    int y, x1, x2, x_intersection;
    int ymin, ymax;
    float w0, w1, w2;
    Color color;
    Vector p;

    ymin = arr[0].coords.y;
    ymax = arr[0].coords.y;
    for (i = 1; i < len; i++) {
        if (arr[i].coords.y < ymin)
            ymin = arr[i].coords.y;
        if (arr[i].coords.y > ymax)
            ymax = arr[i].coords.y;
    }

    for (y = ymin; y <= ymax; y++) {
        x1 = t->width - 1;
        x2 = 0;

        for (i = 0; i < len; i++) {
            j = (i + 1) % len;
            if ((arr[i].coords.y <= y && arr[j].coords.y > y) || (arr[j].coords.y <= y && arr[i].coords.y > y)) {
                x_intersection = arr[i].coords.x + (y - arr[i].coords.y) * (arr[j].coords.x - arr[i].coords.x) / (arr[j].coords.y - arr[i].coords.y);

                if (x_intersection < x1)
                    x1 = x_intersection;
                if (x_intersection > x2)
                    x2 = x_intersection;
            }
        }

        if (x1 <= x2) {
            for (x = x1; x <= x2; x++) {
                p.x = x;
                p.y = y;
                calculate_barycentric_weights(p, &arr[0].coords, &arr[1].coords, &arr[2].coords, &w0, &w1, &w2);

                color = interpolate_color_barycentric(&arr[0], &arr[1], &arr[2], w0, w1, w2);

                draw_point(t, color, x, y);
            }
        }
    }
}
*/
Color interpolate_color_barycentric(const Vertex* v0, const Vertex* v1, const Vertex* v2, float w0, float w1, float w2)
{
    Color color;
    color.r = (GLubyte)(v0->color.r * w0 + v1->color.r * w1 + v2->color.r * w2);
    color.g = (GLubyte)(v0->color.g * w0 + v1->color.g * w1 + v2->color.g * w2);
    color.b = (GLubyte)(v0->color.b * w0 + v1->color.b * w1 + v2->color.b * w2);
    color.a = (GLubyte)(v0->color.a * w0 + v1->color.a * w1 + v2->color.a * w2);
    return color;
}

void calculate_barycentric_weights(Vector p, const Vector* p0, const Vector* p1, const Vector* p2, float* w0, float* w1, float* w2)
{
    int denominator = (p1->y - p2->y) * (p0->x - p2->x) + (p2->x - p1->x) * (p0->y - p2->y);
    *w0 = (float)((p1->y - p2->y) * (p.x - p2->x) + (p2->x - p1->x) * (p.y - p2->y)) / denominator;
    *w1 = (float)((p2->y - p0->y) * (p.x - p2->x) + (p0->x - p2->x) * (p.y - p2->y)) / denominator;
    *w2 = 1.0f - *w0 - *w1;
}

int is_point_in_triangle(Vector p, Vector v1, Vector v2, Vector v3)
{
    float d1, d2, d3;
    int has_neg, has_pos;

    d1 = (p.x - v2.x) * (v1.y - v2.y) - (v1.x - v2.x) * (p.y - v2.y);
    d2 = (p.x - v3.x) * (v2.y - v3.y) - (v2.x - v3.x) * (p.y - v3.y);
    d3 = (p.x - v1.x) * (v3.y - v1.y) - (v3.x - v1.x) * (p.y - v1.y);

    has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}

int is_ear(const Vertex* polygon, int num_vertices, int v0, int v1, int v2)
{
    Vector p0 = polygon[v0].coords;
    Vector p1 = polygon[v1].coords;
    Vector p2 = polygon[v2].coords;
    Vector pi;
    int i;

    /* Check if the triangle is valid and does not have collinear vertices */
    if (p0.x == p1.x && p0.y == p1.y) return 0;
    if (p1.x == p2.x && p1.y == p2.y) return 0;
    if (p2.x == p0.x && p2.y == p0.y) return 0;

    for (i = 0; i < num_vertices; i++)
    {
        if (i == v0 || i == v1 || i == v2) continue;
        pi = polygon[i].coords;
        if (is_point_in_triangle(pi, p0, p1, p2))
        {
            return 0;
        }
    }
    return 1;
}

void ear_clip_triangulate(Texture* texture, Vertex* polygon, int num_vertices)
{
    int* indices = malloc(num_vertices * sizeof(int));
    int i, j, remaining_vertices, prev, curr, next, y, x1, x2, x;
    int ear_found;
    Vertex v0, v1, v2;
    Vector p;
    float w0, w1, w2;
    Color color;

    for (i = 0; i < num_vertices; i++)
    {
        indices[i] = i;
    }

    remaining_vertices = num_vertices;
    while (remaining_vertices > 3)
    {
        ear_found = 0;
        for (i = 0; i < remaining_vertices; i++)
        {
            prev = (i - 1 + remaining_vertices) % remaining_vertices;
            curr = i;
            next = (i + 1) % remaining_vertices;

            if (is_ear(polygon, remaining_vertices, indices[prev], indices[curr], indices[next]))
            {
                /* Draw the triangle */
                v0 = polygon[indices[prev]];
                v1 = polygon[indices[curr]];
                v2 = polygon[indices[next]];

                for (y = min(min(v0.coords.y, v1.coords.y), v2.coords.y); y <= max(max(v0.coords.y, v1.coords.y), v2.coords.y); y++)
                {
                    x1 = min(min(v0.coords.x, v1.coords.x), v2.coords.x);
                    x2 = max(max(v0.coords.x, v1.coords.x), v2.coords.x);
                    for (x = x1; x <= x2; x++)
                    {
                        p.x = x;
                        p.y = y;
                        if (is_point_in_triangle(p, v0.coords, v1.coords, v2.coords))
                        {
                            calculate_barycentric_weights(p, &v0.coords, &v1.coords, &v2.coords, &w0, &w1, &w2);
                            color = interpolate_color_barycentric(&v0, &v1, &v2, w0, w1, w2);
                            draw_point(texture, color, x, y);
                        }
                    }
                }

                /* Remove the ear vertex */
                for (j = curr; j < remaining_vertices - 1; j++)
                {
                    indices[j] = indices[j + 1];
                }
                remaining_vertices--;

                ear_found = 1;
                break;
            }
        }

        if (!ear_found)
        {
            fprintf(stderr, "Failed to find an ear. The polygon might be complex or self-intersecting.\n");
            free(indices);
            return;
        }
    }

    /* Draw the last remaining triangle */
    v0 = polygon[indices[0]];
    v1 = polygon[indices[1]];
    v2 = polygon[indices[2]];

    for (y = min(min(v0.coords.y, v1.coords.y), v2.coords.y); y <= max(max(v0.coords.y, v1.coords.y), v2.coords.y); y++)
    {
        x1 = min(min(v0.coords.x, v1.coords.x), v2.coords.x);
        x2 = max(max(v0.coords.x, v1.coords.x), v2.coords.x);
        for (x = x1; x <= x2; x++)
        {
            p.x = x;
            p.y = y;
            if (is_point_in_triangle(p, v0.coords, v1.coords, v2.coords))
            {
                calculate_barycentric_weights(p, &v0.coords, &v1.coords, &v2.coords, &w0, &w1, &w2);
                color = interpolate_color_barycentric(&v0, &v1, &v2, w0, w1, w2);
                draw_point(texture, color, x, y);
            }
        }
    }

    free(indices);
}

static void draw_shape_full(Texture* t, Vertex arr[], int len)
{
    /* Draw the shape by triangulating and then filling each triangle */
    ear_clip_triangulate(t, arr, len);
}
