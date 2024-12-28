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
