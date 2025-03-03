#include "cimmerian.h"

static void		wall_flat_color(t_frame *f, t_map *m, t_ray *r);
static void		wall_texturing(t_frame *f, t_map *m, t_ray *r);
static t_spr	*get_texture(t_map *m, t_ray *r);

void	draw_wall(t_frame *f, t_map *m, t_ray *r)
{
	if (r->perp_wall_dist > m->dof * g_man.tex_in_dof)
		wall_flat_color(f, m, r);
	else
		wall_texturing(f, m, r);
	return ;
}

static void	wall_flat_color(t_frame *f, t_map *m, t_ray *r)
{
	t_vert	v1;
	t_vert	v2;
	t_color	color;

	v1.coord = r->coord1;
	v2.coord = r->coord2;
	color = m->cells[r->m_index.y * m->size.x + r->m_index.x]
		.tex_north->average_color[0];
	if (r->side == 1)
	{
		color.r /= 2;
		color.g /= 2;
		color.b /= 2;
	}
	apply_wall_fog(&color, m->fog_color, r->perp_wall_dist, m->dof);
	v1.color = color;
	v2.color = color;
	draw_line(f, v1, v2);
	return ;
}

static t_spr	*get_texture(t_map *m, t_ray *r)
{
	if (r->side == 1 && r->ray_dir.y > 0)
		return (m->cells[r->m_index.y * m->size.x + r->m_index.x].tex_north);
	else if (r->side == 1 && r->ray_dir.y < 0)
		return (m->cells[r->m_index.y * m->size.x + r->m_index.x].tex_south);
	else if (r->side == 0 && r->ray_dir.x > 0)
		return (m->cells[r->m_index.y * m->size.x + r->m_index.x].tex_west);
	else if (r->side == 0 && r->ray_dir.x < 0)
		return (m->cells[r->m_index.y * m->size.x + r->m_index.x].tex_east);
	return (0);
}

static void	wall_texturing(t_frame *f, t_map *m, t_ray *r)
{
	t_spr	*tex;
	t_ivec2	tex_coord;
	double	tex_step;
	double	tex_pos;
	t_color	color;
	int		y;

	tex = get_texture(m, r);
	if (!tex)
		return ;

	// tex_coord.x
	double	wall_x;
	if (r->side == 0)
		wall_x = g_man.player.pos.y + r->perp_wall_dist * r->ray_dir.y;
	else
		wall_x = g_man.player.pos.x + r->perp_wall_dist * r->ray_dir.x;
	wall_x -= f_floor(wall_x);
	tex_coord.x = (int)(wall_x * tex->size.x);
	if ((r->side == 0 && r->ray_dir.x < 0)
		|| (r->side == 1 && r->ray_dir.y > 0))
		tex_coord.x = tex->size.x - tex_coord.x - 1;

	// tex_step
	tex_step = (double)tex->size.y / r->line_height_cubic;

	// tex_pos
	t_cell	*cell;
	double	y_offset;
	cell = &m->cells[r->m_index.y * m->size.x + r->m_index.x];
	y_offset = tex->size.y * (1.0 - cell->height) * 0.5;
	tex_pos = (y_offset
		+ (r->coord1.y - (f->size.y * 0.5 - r->line_height * 0.5)) * tex_step);

	int	is_a_corner = is_corner(m, r, tex_coord.x, tex->size.x);

	y = r->coord1.y;
	while (y < r->coord2.y)
	{
		// Cast the image coordinate to integer, and clamp to [0, IMG_H - 1]
		tex_coord.y = (int)tex_pos % tex->size.y;
		tex_pos += tex_step;
		// Clamp the texture coordinate to within bounds
		if (tex_coord.y < 0)
			tex_coord.y += tex->size.y;

		color = tex->cycle[tex->cycle_index][tex_coord.y * tex->size.x + tex_coord.x];
		apply_wall_shadow(&color, m->fog_color, y, r->unclamped_line_height);
		if (is_a_corner)
			apply_corner_shadow(&color, m->fog_color, tex_coord.x, tex->size.x);
		apply_wall_fog(&color, m->fog_color, r->perp_wall_dist, m->dof);
		draw_point(f, color, r->coord1.x, y);
		++y;
	}
	return ;
}
