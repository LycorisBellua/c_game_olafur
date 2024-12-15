#include "cimmerian.h"

t_img	*load_image_from_file(const char *png_path)
{
	t_img			*img;
	t_uivec2		size;
	unsigned int	err;

	img = malloc(sizeof(t_img));
	if (!img)
		return (0);
	err = lodepng_decode32_file(&img->buf, &size.x, &size.y, png_path);
	if (err)
	{
		dprintf(STDERR_FILENO, "Lodepng error n°%u for \"%s\": %s\n",
			err, png_path, lodepng_error_text(err));
		free_image(img);
		return (0);
	}
	img->size.x = size.x;
	img->size.y = size.y;
	img->average_color = calculate_average_color(img);
	img->is_see_through = img->average_color.a < 255;
	return (img);
}

/* Size can be 128x128 */
t_img	*create_image(t_color c, t_ivec2 size)
{
	t_img	*img;
	int		i;

	img = malloc(sizeof(t_img));
	if (!img)
		return (0);
	img->size.x = size.x;
	img->size.y = size.y;
	img->buf = malloc(img->size.x * img->size.y * 4 * sizeof(GLubyte));
	if (!img->buf)
	{
		free_image(img);
		return (0);
	}
	i = 0;
	while (i < img->size.x * img->size.y)
		*((t_color *)img->buf + i++) = c;
	img->average_color = c;
	img->is_see_through = img->average_color.a < 255;
	return (img);
}

void	apply_vertical_gradient(t_img *img, t_color color)
{
	int		x;
	int		y;
	double	gradient_strength;
	double	dist_from_edge;
	double	factor;
	t_color	*buf;
	t_color	*pixel;

	gradient_strength = 0.6;
	buf = (t_color *)img->buf;
	y = 0;
	while (y < img->size.y)
	{
		dist_from_edge = f_abs((double)y - img->size.y / 2) / (img->size.y / 2);
		factor = f_pow(dist_from_edge, 2.0) * gradient_strength;
		x = 0;
		while (x < img->size.x)
		{
			pixel = &buf[y * img->size.x + x];
			pixel->r = (GLubyte)((1.0 - factor) * pixel->r + factor * color.r);
			pixel->g = (GLubyte)((1.0 - factor) * pixel->g + factor * color.g);
			pixel->b = (GLubyte)((1.0 - factor) * pixel->b + factor * color.b);
			pixel->a = (GLubyte)((1.0 - factor) * pixel->a + factor * color.a);
			++x;
		}
		++y;
	}
	return ;
}

void	free_image(t_img *img)
{
	if (img)
		free(img->buf);
	free(img);
	return ;
}
