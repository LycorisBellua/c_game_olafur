#include "cimmerian.h"

t_img	*load_image_from_file(const char *png_path)
{
	t_img			*img;
	t_uivec2		size;
	unsigned int	err;

	img = malloc(sizeof(t_img));
	if (!img)
		return (0);
	err = lodepng_decode32_file((t_ubyte **)&img->buf, &size.x, &size.y,
			png_path);
	if (err)
	{
		dprintf(STDERR_FILENO, "Lodepng error n°%u for \"%s\": %s\n",
			err, png_path, lodepng_error_text(err));
		free_image(img);
		return (0);
	}
	img->size.x = size.x;
	img->size.y = size.y;
	img->average_color = calculate_average_color(img->buf,
			img->size.x * img->size.y);
	img->is_see_through = img->average_color.a < 255;
	return (img);
}

void	free_image(t_img *img)
{
	if (img)
		free(img->buf);
	free(img);
	return ;
}

void	apply_vertical_gradient(t_img *img, t_color color)
{
	t_ivec2	v;
	double	gradient_strength;
	double	edge_dist;
	double	factor;
	t_color	*pixel;

	gradient_strength = 0.6;
	v.y = 0;
	while (v.y < img->size.y)
	{
		edge_dist = f_abs((double)v.y - img->size.y / 2) / (img->size.y / 2);
		factor = f_pow(edge_dist, 2.0) * gradient_strength;
		v.x = 0;
		while (v.x < img->size.x)
		{
			pixel = &img->buf[v.y * img->size.x + v.x];
			pixel->r = (t_ubyte)((1.0 - factor) * pixel->r + factor * color.r);
			pixel->g = (t_ubyte)((1.0 - factor) * pixel->g + factor * color.g);
			pixel->b = (t_ubyte)((1.0 - factor) * pixel->b + factor * color.b);
			pixel->a = (t_ubyte)((1.0 - factor) * pixel->a + factor * color.a);
			++v.x;
		}
		++v.y;
	}
	return ;
}
