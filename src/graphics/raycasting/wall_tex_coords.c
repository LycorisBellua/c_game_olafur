#include "olafur.h"

int	calculate_tex_coord_x(t_img *tex, t_ray *r)
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

float	calculate_initial_tex_pos(t_man *man, t_ray *r, t_img *tex)
{
	t_cell	*cell;
	float	y_offset;
	float	tex_step;

	cell = &r->m->cells[r->m_index.y][r->m_index.x];
	y_offset = tex->size.y * (1.0f - cell->height) * 0.5f;
	tex_step = (float)tex->size.y / r->line_height_cubic;
	return (y_offset + (r->coord1.y - (man->frame.size.y * 0.5f
		- r->line_height * 0.5f)) * tex_step);
}

t_ivec2	calculate_texel_range(t_ray *r, t_img *tex)
{
	t_cell	*cell;
	float	y_offset;
	t_ivec2	range;

	cell = &r->m->cells[r->m_index.y][r->m_index.x];
	y_offset = tex->size.y * (1.0f - cell->height) * 0.5f;
	range.x = (int)floorf(y_offset);
	range.y = (int)ceilf(y_offset + cell->height * tex->size.y) - 1;
	return (range);
}
