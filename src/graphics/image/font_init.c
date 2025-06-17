#include "olafur.h"

static t_ivec2	*get_outline_offsets(void);
static void		set_text_to_white(t_img *img, size_t cycle_index);
static void		add_outline(t_img *img, size_t cycle_index, t_ivec2 *offsets);
static int		cmpc(t_color a, t_color b);

/* White text and black outline */
void	add_outline_to_font(t_img *font)
{
	size_t	cycle_index;
	t_ivec2	*offsets;

	offsets = get_outline_offsets();
	if (!offsets)
		return ;
	cycle_index = 0;
	while (cycle_index < font->cycle_len)
	{
		set_text_to_white(font, cycle_index);
		add_outline(font, cycle_index, offsets);
		++cycle_index;
	}
	free(offsets);
	return ;
}

static t_ivec2	*get_outline_offsets(void)
{
	t_ivec2	*offsets;

	offsets = calloc(8, sizeof(t_ivec2));
	if (!offsets)
		return (0);
	set_ivec2(offsets + 0, -1, 0);
	set_ivec2(offsets + 1, 1, 0);
	set_ivec2(offsets + 2, 0, -1);
	set_ivec2(offsets + 3, 0, 1);
	set_ivec2(offsets + 4, -1, -1);
	set_ivec2(offsets + 5, 1, -1);
	set_ivec2(offsets + 6, -1, 1);
	set_ivec2(offsets + 7, 1, 1);
	return (offsets);
}

static void	set_text_to_white(t_img *img, size_t cycle_index)
{
	size_t	i;
	size_t	len;

	i = 0;
	len = img->size.x * img->size.y;
	while (i < len)
	{
		if (img->cycle[cycle_index][i].a)
			img->cycle[cycle_index][i] = get_color_rgba(255, 255, 255, 255);
		++i;
	}
	return ;
}

static void	add_outline(t_img *img, size_t cycle_index, t_ivec2 *offsets)
{
	int		i;
	int		j;
	t_ivec2	p;

	i = -1;
	while (++i < img->size.x * img->size.y)
	{
		if (cmpc(img->cycle[cycle_index][i],
			get_color_rgba(255, 255, 255, 255)))
		{
			j = -1;
			while (++j < 8)
			{
				p.y = i / img->size.x;
				p.x = i - p.y * img->size.x;
				set_ivec2(&p, p.x + offsets[j].x, p.y + offsets[j].y);
				if (!cmpc(img->cycle[cycle_index][p.y * img->size.x + p.x],
					get_color_rgba(255, 255, 255, 255)))
					img->cycle[cycle_index][p.y * img->size.x + p.x] = \
get_color_rgba(0, 0, 0, 255);
			}
		}
	}
	return ;
}

static int	cmpc(t_color a, t_color b)
{
	return (a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a);
}
