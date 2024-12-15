#include "cimmerian.h"

static int	hex_char_to_int(char c);

t_color	get_color_rgba(GLubyte r, GLubyte g, GLubyte b, GLubyte a)
{
	t_color	c;

	c.r = r;
	c.g = g;
	c.b = b;
	c.a = a;
	return (c);
}

t_color	get_color_hex(const char *str, GLubyte alpha)
{
	int		i;
	t_color	c;

	i = str[0] == '#';
	c.r = hex_char_to_int(str[i + 0]) * 16 + hex_char_to_int(str[i + 1]);
	c.g = hex_char_to_int(str[i + 2]) * 16 + hex_char_to_int(str[i + 3]);
	c.b = hex_char_to_int(str[i + 4]) * 16 + hex_char_to_int(str[i + 5]);
	c.a = alpha;
	return (c);
}

t_color	get_alpha_blended_color(t_color prev, t_color new)
{
	t_color	blend;

	blend.a = new.a + (255 - new.a) * prev.a / 255;
	if (!blend.a)
		return (get_color_rgba(0, 0, 0, 0));
	blend.r = (new.a * new.r + (255 - new.a) * prev.a * prev.r / 255) / blend.a;
	blend.g = (new.a * new.g + (255 - new.a) * prev.a * prev.g / 255) / blend.a;
	blend.b = (new.a * new.b + (255 - new.a) * prev.a * prev.b / 255) / blend.a;
	return (blend);
}

int	cmp_color(t_color a, t_color b)
{
	return (a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a);
}

t_color	calculate_average_color(t_img *img)
{
	int		i;
	int		len;
	int		total_color[4];
	t_color	*buf;
	t_color	average;
	double	alpha;

	total_color[0] = 0;
	total_color[1] = 0;
	total_color[2] = 0;
	total_color[3] = 0;
	buf = (t_color *)img->buf;
	len = img->size.x * img->size.y;
	i = 0;
	while (i < len)
	{
		alpha = buf[i].a / 255.0;
		total_color[0] += buf[i].r * alpha;
		total_color[1] += buf[i].g * alpha;
		total_color[2] += buf[i].b * alpha;
		total_color[3] += buf[i].a;
		++i;
	}
	average.r = total_color[0] / len;
	average.g = total_color[1] / len;
	average.b = total_color[2] / len;
	average.a = total_color[3] / len;
	return (average);
}

t_color	get_frame_color(t_frame *f, int x, int y)
{
	x *= f->thickness;
	y *= f->thickness;
	return (((t_color *)f->buf)[y * f->real_size.x + x]);
}

static int	hex_char_to_int(char c)
{
	if (isdigit(c))
		return (c - '0');
	else if (c >= 'a' && c <= 'f')
		return (10 + (c - 'a'));
	else if (c >= 'A' && c <= 'F')
		return (10 + (c - 'A'));
	return (0);
}
