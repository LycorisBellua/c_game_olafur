#include "olafur.h"

static int		calculate_tex_coord_x(t_img *tex, t_ray *r);
static float	calculate_initial_tex_pos(t_man *man, t_ray *r, t_img *tex);
static t_color	shade_wall_pixel(t_ray *r, t_color c, t_color fog, int k);
static int		wall_pixel_occluded(t_man *man, t_ray *r);

void	draw_wall(t_man *man, t_ray *r, t_img *tex)
{
	int		y;
	int		it_prev;
	int		it_new;
	t_ivec2	tex_coord;
	float	tex_step;
	float	tex_pos;
	t_color	*frame;
	int		tex_h;
	int		col_base;
	t_color	shade_fog;
	int		shade_k;

	if (!tex)
		return ;
	tex_coord.x = calculate_tex_coord_x(tex, r);
	r->corner_factor8 = (int)(corner_intensity_of(r, tex, tex_coord.x) * 256.0f);
	set_wall_shadow_params(r);
	frame = tex->cycle[tex->cycle_index];
	tex_h = tex->size.y;
	col_base = tex_coord.x * tex_h;
	shade_fog = r->m->fog_color;
	shade_k = (256 - r->corner_factor8) * (256 - r->fog_factor8);
	tex_step = (float)tex_h / r->line_height_cubic;
	tex_pos = calculate_initial_tex_pos(man, r, tex);
	it_prev = (int)tex_pos;
	tex_coord.y = it_prev % tex_h;
	if (tex_coord.y < 0)
		tex_coord.y += tex_h;
	y = r->coord1.y;
	while (r->coord1.y <= r->coord2.y)
	{
		if (!wall_pixel_occluded(man, r))
			draw_point_fast(man, shade_wall_pixel(r,
					frame[col_base + tex_coord.y], shade_fog, shade_k),
				r->coord1.x, r->coord1.y);
		tex_pos += tex_step;
		it_new = (int)tex_pos;
		tex_coord.y += it_new - it_prev;
		it_prev = it_new;
		while (tex_coord.y >= tex_h)
			tex_coord.y -= tex_h;
		++r->coord1.y;
	}
	r->coord1.y = y;
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

static int	calculate_tex_coord_x(t_img *tex, t_ray *r)
{
	int		tex_coord_x;
	float	wall_x;

	if (r->side == 0)
		wall_x = r->origin_pos.y + r->perp_wall_dist * r->ray_dir.y;
	else
		wall_x = r->origin_pos.x + r->perp_wall_dist * r->ray_dir.x;
	wall_x -= floorf(wall_x);
	tex_coord_x = (int)(wall_x * tex->size.x);
	if ((r->side == 0 && r->ray_dir.x < 0)
		|| (r->side == 1 && r->ray_dir.y > 0))
		tex_coord_x = tex->size.x - tex_coord_x - 1;
	if (r->invert_tex)
		tex_coord_x = tex->size.x - tex_coord_x - 1;
	return (tex_coord_x);
}

static float	calculate_initial_tex_pos(t_man *man, t_ray *r, t_img *tex)
{
	t_cell	*cell;
	float	y_offset;
	float	tex_step;

	cell = &r->m->cells[r->m_index.y][r->m_index.x];
	y_offset = tex->size.y * (1.0 - cell->height) * 0.5;
	tex_step = (float)tex->size.y / r->line_height_cubic;
	return (y_offset + (r->coord1.y - (man->frame.size.y * 0.5 - r->line_height
				* 0.5)) * tex_step);
}

static t_color	shade_wall_pixel(t_ray *r, t_color c, t_color fog, int k)
{
	int		s8;
	int		g8;

	s8 = wall_shadow_factor8(r, r->coord1.y);
	g8 = 256 - (((256 - s8) * k) >> 16);
	if (c.a > 0)
		c.a = ((256 - s8) * c.a + s8 * fog.a) >> 8;
	c.r = ((256 - g8) * c.r + g8 * fog.r) >> 8;
	c.g = ((256 - g8) * c.g + g8 * fog.g) >> 8;
	c.b = ((256 - g8) * c.b + g8 * fog.b) >> 8;
	return (c);
}
