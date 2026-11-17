#include "olafur.h"

static void		init_wall_tex(t_man *man, t_ray *r, t_img *tex, t_wall_tex *w);
static int		wall_pixel_occluded(t_man *man, t_ray *r);
static t_color	shade_wall_pixel(t_ray *r, t_wall_tex *w);

void	draw_wall(t_man *man, t_ray *r, t_img *tex)
{
	t_wall_tex	w;
	int			y;
	int			it_new;

	if (!tex)
		return ;
	init_wall_tex(man, r, tex, &w);
	y = r->coord1.y;
	while (r->coord1.y <= r->coord2.y)
	{
		if (!wall_pixel_occluded(man, r))
			draw_point_fast(man, shade_wall_pixel(r, &w), r->coord1.x,
				r->coord1.y);
		w.pos += w.step;
		it_new = imin(imax((int)floorf(w.pos), w.range.x), w.range.y);
		w.tex_y += it_new - w.it_prev;
		w.it_prev = it_new;
		while (w.tex_y >= w.tex_h)
			w.tex_y -= w.tex_h;
		++r->coord1.y;
	}
	r->coord1.y = y;
	return ;
}

static void	init_wall_tex(t_man *man, t_ray *r, t_img *tex, t_wall_tex *w)
{
	int	tex_x;

	tex_x = calculate_tex_coord_x(tex, r);
	r->corner_factor8 = (int)(corner_intensity_of(r, tex, tex_x) * 256.0f);
	set_wall_shadow_params(r);
	w->frame = tex->cycle[tex->cycle_index];
	w->tex_h = tex->size.y;
	w->col_base = tex_x * w->tex_h;
	w->fog = r->m->fog_color;
	w->k = (256 - r->corner_factor8) * (256 - r->fog_factor8);
	w->step = (float)w->tex_h / r->line_height_cubic;
	w->pos = calculate_initial_tex_pos(man, r, tex);
	w->range = calculate_texel_range(r, tex);
	w->it_prev = imin(imax((int)floorf(w->pos), w->range.x), w->range.y);
	w->tex_y = w->it_prev % w->tex_h;
	if (w->tex_y < 0)
		w->tex_y += w->tex_h;
	return ;
}

static int	wall_pixel_occluded(t_man *man, t_ray *r)
{
	int	idx;

	if (r->coord1.y >= man->frame.size.y / 2)
		return (0);
	idx = r->coord1.y * man->frame.size.x + r->coord1.x;
	return (r->perp_wall_dist > man->fc_depth[idx]);
}

static t_color	shade_wall_pixel(t_ray *r, t_wall_tex *w)
{
	t_color	c;
	int		s8;
	int		g8;

	c = w->frame[w->col_base + w->tex_y];
	s8 = wall_shadow_factor8(r, r->coord1.y);
	g8 = 256 - (((256 - s8) * w->k) >> 16);
	if (c.a > 0)
		c.a = ((256 - s8) * c.a + s8 * w->fog.a) >> 8;
	c.r = ((256 - g8) * c.r + g8 * w->fog.r) >> 8;
	c.g = ((256 - g8) * c.g + g8 * w->fog.g) >> 8;
	c.b = ((256 - g8) * c.b + g8 * w->fog.b) >> 8;
	return (c);
}
