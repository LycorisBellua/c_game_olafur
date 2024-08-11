#include "cimmerian.h"

static void ear_clip_triangulate(Texture* texture, Vertex* polygon, int num_vertices);
static int is_ear(const Vertex* polygon, int num_vertices, int v0, int v1, int v2);
static int is_point_in_triangle(Vector p, Vector v1, Vector v2, Vector v3);
static void draw_triangle(Texture* texture, const Vertex* v0, const Vertex* v1, const Vertex* v2);
static void calculate_barycentric_weights(Vector p, const Vector* p0, const Vector* p1, const Vector* p2, float* w0, float* w1, float* w2);
static Color interpolate_color_barycentric(const Vertex* v0, const Vertex* v1, const Vertex* v2, float w0, float w1, float w2);

void draw_shape(Texture* t, Vertex arr[], int len)
{
    int i;

    i = 0;
    while (i < len)
    {
        draw_line(t, arr[i], arr[(i + 1) % len]);
        ++i;
    }
    return;
}

void draw_shape_full(Texture* t, Vertex arr[], int len)
{
    if (len < 3)
    {
        draw_shape(t, arr, len);
        return;
    }
    ear_clip_triangulate(t, arr, len);
    return;
}

static void ear_clip_triangulate(Texture* texture, Vertex* polygon, int num_vertices)
{
    int* indices;
    int remaining_vertices;
    int ear_found;
    int i;
    int j;
    int prev;
    int curr;
    int next;

    indices = malloc(num_vertices * sizeof(int));
    remaining_vertices = num_vertices;
    i = 0;
    while (i < num_vertices)
    {
        indices[i] = i;
        ++i;
    }
    while (remaining_vertices > 3)
    {
        ear_found = 0;
        i = 0;
        while (i < remaining_vertices)
        {
            prev = (i - 1 + remaining_vertices) % remaining_vertices;
            curr = i;
            next = (i + 1) % remaining_vertices;
            if (is_ear(polygon, remaining_vertices, indices[prev], indices[curr], indices[next]))
            {
                draw_triangle(texture, &polygon[indices[prev]], &polygon[indices[curr]], &polygon[indices[next]]);
                j = curr;
                while (j < remaining_vertices - 1)
                {
                    indices[j] = indices[j + 1];
                    ++j;
                }
                remaining_vertices--;
                ear_found = 1;
                break;
            }
            ++i;
        }
        if (!ear_found)
        {
            free(indices);
            return;
        }
    }
    draw_triangle(texture, &polygon[indices[0]], &polygon[indices[1]], &polygon[indices[2]]);
    free(indices);
    return;
}

static int is_ear(const Vertex* polygon, int num_vertices, int v0, int v1, int v2)
{
    Vector p0;
    Vector p1;
    Vector p2;
    Vector pi;
    int i;

    p0 = polygon[v0].coords;
    p1 = polygon[v1].coords;
    p2 = polygon[v2].coords;
    if ((p0.x == p1.x && p0.y == p1.y)
        || (p1.x == p2.x && p1.y == p2.y)
        || (p2.x == p0.x && p2.y == p0.y))
        return 0;
    i = 0;
    while (i < num_vertices)
    {
        if (i != v0 && i != v1 && i != v2)
        {
            pi = polygon[i].coords;
            if (is_point_in_triangle(pi, p0, p1, p2))
                return 0;
        }
        ++i;
    }
    return 1;
}

static int is_point_in_triangle(Vector p, Vector v1, Vector v2, Vector v3)
{
    float d1;
    float d2;
    float d3;
    int has_neg;
    int has_pos;

    d1 = (p.x - v2.x) * (v1.y - v2.y) - (v1.x - v2.x) * (p.y - v2.y);
    d2 = (p.x - v3.x) * (v2.y - v3.y) - (v2.x - v3.x) * (p.y - v3.y);
    d3 = (p.x - v1.x) * (v3.y - v1.y) - (v3.x - v1.x) * (p.y - v1.y);
    has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);
    return !(has_neg && has_pos);
}

static void draw_triangle(Texture* texture, const Vertex* v0, const Vertex* v1, const Vertex* v2)
{
    int y;
    int x;
    float w0;
    float w1;
    float w2;
    Vector p;
    Color color;

    y = min(min(v0->coords.y, v1->coords.y), v2->coords.y);
    while (y <= max(max(v0->coords.y, v1->coords.y), v2->coords.y))
    {
        x = min(min(v0->coords.x, v1->coords.x), v2->coords.x);
        while (x <= max(max(v0->coords.x, v1->coords.x), v2->coords.x))
        {
            p.x = x;
            p.y = y;
            if (is_point_in_triangle(p, v0->coords, v1->coords, v2->coords))
            {
                calculate_barycentric_weights(p, &v0->coords, &v1->coords, &v2->coords, &w0, &w1, &w2);
                color = interpolate_color_barycentric(v0, v1, v2, w0, w1, w2);
                draw_point(texture, color, x, y);
            }
            ++x;
        }
        ++y;
    }
    return;
}

static void calculate_barycentric_weights(Vector p, const Vector* p0, const Vector* p1, const Vector* p2, float* w0, float* w1, float* w2)
{
    float denominator;
    float inv_denom;

    denominator = (p1->y - p2->y) * (p0->x - p2->x) + (p2->x - p1->x) * (p0->y - p2->y);
    inv_denom = 1.0f / denominator;
    *w0 = ((p1->y - p2->y) * (p.x - p2->x) + (p2->x - p1->x) * (p.y - p2->y)) * inv_denom;
    *w1 = ((p2->y - p0->y) * (p.x - p2->x) + (p0->x - p2->x) * (p.y - p2->y)) * inv_denom;
    *w2 = 1.0f - *w0 - *w1;
    return;
}

static Color interpolate_color_barycentric(const Vertex* v0, const Vertex* v1, const Vertex* v2, float w0, float w1, float w2)
{
    Color color;

    color.r = (GLubyte)(v0->color.r * w0 + v1->color.r * w1 + v2->color.r * w2);
    color.g = (GLubyte)(v0->color.g * w0 + v1->color.g * w1 + v2->color.g * w2);
    color.b = (GLubyte)(v0->color.b * w0 + v1->color.b * w1 + v2->color.b * w2);
    color.a = (GLubyte)(v0->color.a * w0 + v1->color.a * w1 + v2->color.a * w2);
    return color;
}
