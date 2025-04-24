#include "cimmerian.h"

static t_img	*get_texture(t_map *m, t_ray *r, int is_portal);
static int		calculate_tex_coord_x(t_man *man, t_img *tex, t_ray *r);
static double	calculate_initial_tex_pos(t_man *man, t_map *m, t_ray *r,
					t_img *tex);
static t_color	calculate_color(t_man *man, t_ray *r, t_img *tex,
					t_ivec2 tex_coord);

void	draw_wall(t_man *man, t_ray *r, int is_portal)
{
	int		y;
	t_img	*tex;
	t_ivec2	tex_coord;
	double	tex_step;
	double	tex_pos;

	tex = get_texture(man->maps[man->curr_map], r, is_portal);
	if (!tex)
		return ;
	tex_coord.x = calculate_tex_coord_x(man, tex, r);
	tex_step = (double)tex->size.y / r->line_height_cubic;
	tex_pos = calculate_initial_tex_pos(man, man->maps[man->curr_map], r, tex);
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

static t_img	*get_texture(t_map *m, t_ray *r, int is_portal)
{
	t_portal	*p;

	if (is_portal)
	{
		p = m->cells[r->m_index.y][r->m_index.x].portal;
		if (!p)
			return (0);
		else if (p->src_cardinal == 'N' && r->side == 1 && r->ray_dir.y > 0)
			return (p->tex);
		else if (p->src_cardinal == 'S' && r->side == 1 && r->ray_dir.y < 0)
			return (p->tex);
		else if (p->src_cardinal == 'W' && r->side == 0 && r->ray_dir.x > 0)
			return (p->tex);
		else if (p->src_cardinal == 'E' && r->side == 0 && r->ray_dir.x < 0)
			return (p->tex);
	}
	else if (r->side == 1 && r->ray_dir.y > 0)
		return (m->cells[r->m_index.y][r->m_index.x].tex_north);
	else if (r->side == 1 && r->ray_dir.y < 0)
		return (m->cells[r->m_index.y][r->m_index.x].tex_south);
	else if (r->side == 0 && r->ray_dir.x > 0)
		return (m->cells[r->m_index.y][r->m_index.x].tex_west);
	else if (r->side == 0 && r->ray_dir.x < 0)
		return (m->cells[r->m_index.y][r->m_index.x].tex_east);
	return (0);
}

static int	calculate_tex_coord_x(t_man *man, t_img *tex, t_ray *r)
{
	int		tex_coord_x;
	double	wall_x;

	if (r->side == 0)
		wall_x = man->player.pos.y + r->perp_wall_dist * r->ray_dir.y;
	else
		wall_x = man->player.pos.x + r->perp_wall_dist * r->ray_dir.x;
	wall_x -= floor(wall_x);
	tex_coord_x = (int)(wall_x * tex->size.x);
	if ((r->side == 0 && r->ray_dir.x < 0)
		|| (r->side == 1 && r->ray_dir.y > 0))
		tex_coord_x = tex->size.x - tex_coord_x - 1;
	return (tex_coord_x);
}

static double	calculate_initial_tex_pos(t_man *man, t_map *m, t_ray *r,
	t_img *tex)
{
	t_cell	*cell;
	double	y_offset;
	double	tex_step;

	cell = &m->cells[r->m_index.y][r->m_index.x];
	y_offset = tex->size.y * (1.0 - cell->height) * 0.5;
	tex_step = (double)tex->size.y / r->line_height_cubic;
	return (y_offset + (r->coord1.y - (man->frame.size.y * 0.5 - r->line_height
				* 0.5)) * tex_step);
}

static t_color	calculate_color(t_man *man, t_ray *r, t_img *tex,
	t_ivec2 tex_coord)
{
	static int	is_a_corner = -1;
	t_map		*m;
	t_color		c;

	m = man->maps[man->curr_map];
	if (is_a_corner < 0)
		is_a_corner = is_corner(m, r, tex_coord.x, tex->size.x);
	c = tex->cycle[tex->cycle_index][tex_coord.y * tex->size.x + tex_coord.x];
	apply_wall_shadow(&c, m->fog_color, r->coord1.y, r->unclamped_line_height);
	if (is_a_corner)
		apply_corner_shadow(&c, m->fog_color, tex_coord.x, tex->size.x);
	apply_wall_fog(&c, m->fog_color, r->perp_wall_dist, man->dof);
	if (r->coord1.y == r->coord2.y)
		is_a_corner = -1;
	return (c);
}
