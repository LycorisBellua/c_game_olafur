#include "cimmerian.h"

static int		calculate_tex_coord_x(t_img *tex, t_ray *r);
static double	calculate_initial_tex_pos(t_man *man, t_ray *r, t_img *tex);
static t_color	calculate_color(t_man *man, t_ray *r, t_img *tex,
					t_ivec2 tex_coord);

void	draw_wall(t_man *man, t_ray *r, t_img *tex)
{
	int		y;
	t_ivec2	tex_coord;
	double	tex_step;
	double	tex_pos;

	if (!tex)
		return ;
	tex_coord.x = calculate_tex_coord_x(tex, r);
	tex_step = (double)tex->size.y / r->line_height_cubic;
	tex_pos = calculate_initial_tex_pos(man, r, tex);
	y = r->coord1.y;
	while (r->coord1.y <= r->coord2.y)
	{
		tex_coord.y = (int)tex_pos % tex->size.y;
		tex_pos += tex_step;
		if (tex_coord.y < 0)
			tex_coord.y += tex->size.y;
		draw_point(man, calculate_color(man, r, tex, tex_coord), r->coord1.x,
			r->coord1.y);
		++r->coord1.y;
	}
	r->coord1.y = y;
	return ;
}

static int	calculate_tex_coord_x(t_img *tex, t_ray *r)
{
	int		tex_coord_x;
	double	wall_x;

	if (r->side == 0)
		wall_x = r->origin_pos.y + r->perp_wall_dist * r->ray_dir.y;
	else
		wall_x = r->origin_pos.x + r->perp_wall_dist * r->ray_dir.x;
	wall_x -= floor(wall_x);
	tex_coord_x = (int)(wall_x * tex->size.x);
	if ((r->side == 0 && r->ray_dir.x < 0)
		|| (r->side == 1 && r->ray_dir.y > 0))
		tex_coord_x = tex->size.x - tex_coord_x - 1;
	if (r->invert_tex)
		tex_coord_x = tex->size.x - tex_coord_x - 1;
	return (tex_coord_x);
}

static double	calculate_initial_tex_pos(t_man *man, t_ray *r, t_img *tex)
{
	t_cell	*cell;
	double	y_offset;
	double	tex_step;

	cell = &r->m->cells[r->m_index.y][r->m_index.x];
	y_offset = tex->size.y * (1.0 - cell->height) * 0.5;
	tex_step = (double)tex->size.y / r->line_height_cubic;
	return (y_offset + (r->coord1.y - (man->frame.size.y * 0.5 - r->line_height
				* 0.5)) * tex_step);
}

static t_color	calculate_color(t_man *man, t_ray *r, t_img *tex,
	t_ivec2 tex_coord)
{
	static int	is_a_corner = -1;
	t_color		c;

	if (is_a_corner < 0)
		is_a_corner = is_corner(r->m, r, tex_coord.x, tex->size.x);
	c = tex->cycle[tex->cycle_index][tex_coord.y * tex->size.x + tex_coord.x];
	apply_wall_shadow(&c, r->m->fog_color, r->coord1.y,
		r->unclamped_line_height);
	if (is_a_corner)
		apply_corner_shadow(&c, r->m->fog_color, tex_coord.x, tex->size.x);
	apply_wall_fog(&c, r->m->fog_color, r->perp_wall_dist, man->dof);
	if (r->coord1.y == r->coord2.y)
		is_a_corner = -1;
	return (c);
}
