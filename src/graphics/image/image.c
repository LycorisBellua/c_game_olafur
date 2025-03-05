#include "cimmerian.h"

t_png	*load_png_from_path(const char *path)
{
	t_png			*png;
	t_uivec2		size;
	unsigned int	err;

	png = malloc(sizeof(t_png));
	if (!png)
		return (0);
	err = lodepng_decode32_file((t_ubyte **)&png->buf, &size.x, &size.y, path);
	if (err)
	{
		dprintf(STDERR_FILENO, "Lodepng error n°%u for \"%s\": %s\n",
			err, path, lodepng_error_text(err));
		free_png(png);
		return (0);
	}
	png->size.x = size.x;
	png->size.y = size.y;
	return (png);
}

t_png	*create_empty_png(t_ivec2 size)
{
	t_png	*png;

	png = calloc(1, sizeof(t_png));
	if (!png)
		return (0);
	set_ivec2(&png->size, size.x, size.y);
	png->buf = calloc(png->size.x * png->size.y, sizeof(t_color));
	if (!png->buf)
	{
		free_png(png);
		return (0);
	}
	return (png);
}

void	free_png(t_png *png)
{
	if (png)
		free(png->buf);
	free(png);
	return ;
}

void	apply_vertical_gradient(t_png *png, t_color color)
{
	t_ivec2	v;
	double	gradient_strength;
	double	edge_dist;
	double	factor;
	t_color	*pixel;

	gradient_strength = 0.6;
	v.y = 0;
	while (v.y < png->size.y)
	{
		edge_dist = abs_f((double)v.y - png->size.y / 2) / (png->size.y / 2);
		factor = pow_f(edge_dist, 2.0) * gradient_strength;
		v.x = 0;
		while (v.x < png->size.x)
		{
			pixel = &png->buf[v.y * png->size.x + v.x];
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
