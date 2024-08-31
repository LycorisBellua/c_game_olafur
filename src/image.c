#include "cimmerian.h"
#include "lodepng.h"

/* Image size must be a power of two */
t_img* load_image_from_file(char* png_path)
{
    t_img* img;
    t_uivec2 size;
    unsigned int err;
    
    img = malloc(sizeof(t_img));
    if (!img)
        return 0;
    err = lodepng_decode32_file(&img->buf, &size.x, &size.y, png_path);
    if (err)
    {
        fprintf(stderr, "Error: %s (Lodepng lib - error n°%u)\n", 
            lodepng_error_text(err), err);
        free_image(img);
        return 0;
    }
    img->size.x = size.x;
    img->size.y = size.y;
    img->average_color = calculate_average_color(img);
    img->is_see_through = img->average_color.a < 255;
    return img;
}

t_img* create_image(t_color c)
{
    t_img* img;
    int i;

    img = malloc(sizeof(t_img));
    if (!img)
        return 0;
    img->size.x = IMG_W;
    img->size.y = IMG_H;
    img->buf = malloc(img->size.x * img->size.y * 4 * sizeof(GLubyte));
    if (!img->buf)
    {
        free_image(img);
        return 0;
    }
    i = 0;
    while (i < img->size.x * img->size.y)
        *((t_color*)img->buf + i++) = c;
    img->average_color = c;
    img->is_see_through = img->average_color.a < 255;
    return img;
}

void free_image(t_img* img)
{
    free(img->buf);
    free(img);
    return;
}
