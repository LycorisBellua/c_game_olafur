#include "cimmerian.h"

static t_tex* create_texture(void);
static void free_texture(t_tex** t);

void create_textures(void)
{
    int i;

    man.curr_tex = 0;
    i = 0;
    while (i < NBR_TEXTURES)
    {
        man.tex[i] = create_texture();
        ++i;
    }
    man.tex[i] = 0;
    return;
}

void use_texture(t_tex* t)
{
    glBindTexture(GL_TEXTURE_2D, t ? t->id : 0);
    return;
}

void clear_drawing(t_tex* t)
{
    bzero(t->buf, t->real_size.x * t->real_size.y * 4 * sizeof(GLubyte));
    return;
}

void save_drawing(t_tex* t)
{
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, t->real_size.x, t->real_size.y, 0, 
        GL_RGBA, GL_UNSIGNED_BYTE, t->buf);
    return;
}

void free_textures(void)
{
    unsigned int i;

    use_texture(0);

    i = 0;
    while (i < NBR_TEXTURES)
    {
        free_texture(&man.tex[i]);
        ++i;
    }
    return;
}

static t_tex* create_texture(void)
{
    t_tex* t;
    long buf_length;

    t = malloc(sizeof(t_tex));
    if (!t)
    {
        fprintf(stderr, "Error: Couldn't allocate memory for texture");
        return 0;
    }

    t->id = -1;
    t->real_size.x = man.res.monitor_size.x;
    t->real_size.y = man.res.monitor_size.y;
    t->thickness = t->real_size.x/man.res.window_size_default.x;
    t->size.x = t->real_size.x / t->thickness;
    t->size.y = t->real_size.y / t->thickness;

    buf_length = t->real_size.x * t->real_size.y * 4 * sizeof(GLubyte);
    t->buf = malloc(buf_length);
    if (!t->buf)
    {
        fprintf(stderr, "Error: Couldn't allocate enough memory for a texture "
            "(= %ld bytes for texture of a %dx%d resolution)\n", 
            buf_length, t->real_size.x, t->real_size.y);
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

static void free_texture(t_tex** t)
{
    /*
        Check whether the pointer is null. If it is, it means it doesn't 
        reference any struct, which therefore means that `buf` is not a 
        pointer to a legal address.
    */
    if (!t)
        return;

    glDeleteTextures(1, &(*t)->id);
    free((*t)->buf);
    free(*t);

    /* Nullify the reference to prevent a double free */
    *t = 0;
    return;
}
