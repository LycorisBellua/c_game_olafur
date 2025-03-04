#include "cimmerian.h"

static void	add_floor_to_skybox(t_spr *s, t_color fog);
static void	apply_modif_to_curr_cycle_frame(t_spr *s, t_color fog,
				int h_gradient, t_color bottom);

t_spr	*compose_skybox(t_spr *src, t_color fog)
{
	t_spr	*skybox;

	skybox = duplicate_sprite("skybox", src);
	add_floor_to_skybox(skybox, fog);
	return (skybox);
}

static void	add_floor_to_skybox(t_spr *s, t_color fog)
{
	int		h_gradient;
	t_color	bottom;

	if (!s)
		return ;
	h_gradient = s->size.y / 2;
	bottom = get_color_rgba(42, 30, 30, 255);
	while (s->cycle_index < s->cycle_len)
	{
		apply_modif_to_curr_cycle_frame(s, fog, h_gradient, bottom);
		++s->cycle_index;
	}
	s->cycle_index = 0;
	return ;
}

static void	apply_modif_to_curr_cycle_frame(t_spr *s, t_color fog,
	int h_gradient, t_color bottom)
{
	t_vert	v;
	double	factor;

	v.coord.y = s->size.y / 2;
	while (v.coord.y < s->size.y)
	{
		v.coord.x = 0;
		factor = (double)(v.coord.y - s->size.y / 2) / h_gradient;
		while (v.coord.x < s->size.x)
		{
			v.color.r = (1 - factor) * fog.r + factor * bottom.r;
			v.color.g = (1 - factor) * fog.g + factor * bottom.g;
			v.color.b = (1 - factor) * fog.b + factor * bottom.b;
			v.color.a = 255;
			draw_pixel(s->cycle[s->cycle_index], v.color, v.coord, s->size);
			++v.coord.x;
		}
		++v.coord.y;
	}
	return ;
}
