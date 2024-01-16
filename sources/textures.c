#include "../headers/textures.h"
#include "../headers/windowing.h"
#include "../headers/colors.h"

Texture* textures[NBR_TEXTURES] = {0};

static Texture* create_texture(void);
static void free_texture(Texture** t);
static void set_alpha_to_zero(Texture* t);

void create_textures(void)
{
    TEX_MAIN = create_texture();
    textures[NBR_TEXTURES - 1] = 0;
    return;
}

void use_texture(const Texture* t)
{
    glBindTexture(GL_TEXTURE_2D, t ? t->id : 0);
    return;
}

void clear_drawing(Texture* t, const int true_clear)
{
    /* Memset is faster than zeroing the alpha channel */
    if (true_clear)
        memset(t->buffer, 0, t->real_width * t->real_height * 4);
    else
        set_alpha_to_zero(t);
    return;
}

void save_drawing(const Texture* t)
{
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t->real_width, t->real_height, 0, 
        GL_RGBA, GL_UNSIGNED_BYTE, t->buffer);
    return;
}

void free_textures(void)
{
    unsigned int i;

    use_texture(0);

    for (i = 0; i < sizeof(textures)/sizeof(Texture*); ++i)
    {
        if (!textures[i])
            break;
        free_texture(&textures[i]);
    }
    return;
}

static Texture* create_texture(void)
{
    Texture* t = malloc(sizeof(Texture));
    long buffer_length;
    if (!t)
    {
        fprintf(stderr, "ERROR: Couldn't allocate memory for texture.");
        return 0;
    }

    t->id = -1;
    t->real_width = res.monitor_width;
    t->real_height = res.monitor_height;
    t->thickness = t->real_width/res.window_width_default;
    t->width = t->real_width / t->thickness;
    t->height = t->real_height / t->thickness;

    buffer_length = t->real_width * t->real_height * 4 * sizeof(unsigned char);
    t->buffer = malloc(buffer_length);
    if (!t->buffer)
    {
        fprintf(stderr, "ERROR: Couldn't allocate enough memory for a texture "
            "(= %ld bytes for RGBA texture of a %dx%d resolution).\n", 
            buffer_length, t->real_width, t->real_height);
        free(t);
        return 0;
    }

    glGenTextures(1, &t->id);
    glBindTexture(GL_TEXTURE_2D, t->id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);
    return t;
}

static void free_texture(Texture** t)
{
    /*
        Check whether the pointer is null. If it is, it means it doesn't 
        reference any struct, which therefore means that `buffer` is not a 
        pointer to a legal address.
    */
    if (!t)
        return;

    glDeleteTextures(1, &(*t)->id);
    free((*t)->buffer);
    free(*t);

    /* Nullify the reference to prevent a double free */
    *t = 0;
    return;
}

static void set_alpha_to_zero(Texture* t)
{
    unsigned char* ptr = t->buffer + 3;
    unsigned char* end = t->buffer + t->real_width * t->real_height * 4;

    while (ptr < end)
    {
        *ptr = 0;
        ptr += 4;
    }
    return;
}

int is_coord_out_of_bounds(const int axis_length, const int coord)
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

/* Origin is bottom left hand corner */
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

/* Coord is center of shape */
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

/* Test functions ---------------------------------------------------------- */

void draw_corners(Texture* t)
{
    /* Bottom left */
    color_default = &colors[COLOR_WHITE];
    draw_point(t, 0, 0);
    /* Top left */
    color_default = &colors[COLOR_RED];
    draw_point(t, 0, t->height - 1);
    /* Top right */
    color_default = &colors[COLOR_GREEN];
    draw_point(t, t->width - 1, t->height - 1);
    /* Bottom right */
    color_default = &colors[COLOR_BLUE];
    draw_point(t, t->width - 1, 0);

    color_default = &colors[COLOR_WHITE];
    return;
}

void draw_center(Texture* t)
{
    /* Bottom left */
    color_default = &colors[COLOR_WHITE];
    draw_point(t, t->width/2 -1, t->height/2 -1);
    /* Top left */
    color_default = &colors[COLOR_RED];
    draw_point(t, t->width/2 -1, t->height/2);
    /* Top right */
    color_default = &colors[COLOR_GREEN];
    draw_point(t, t->width/2, t->height/2);
    /* Bottom right */
    color_default = &colors[COLOR_BLUE];
    draw_point(t, t->width/2, t->height/2 -1);

    color_default = &colors[COLOR_WHITE];
    return;
}

void draw_gradient(Texture* t)
{
    int x, y;

    Color* color = malloc(sizeof(Color));
    color_default = &color;
    color->b = 255/2;
    color->a = 255;

    for (y = 0; y < t->height; ++y)
    {
        for (x = 0; x < t->width; ++x)
        {
            color->r = x * 255 / t->width;
            color->g = y * 255 / t->height;
            draw_point(t, x, y);
        }
    }

    color_default = &colors[COLOR_WHITE];
    free(color);
    return;
}

