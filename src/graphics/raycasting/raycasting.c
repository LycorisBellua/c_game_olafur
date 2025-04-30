#include "cimmerian.h"

static void	reset_z_buffer(t_man *man);
static void	process_dda_list(t_man *man, int x);
static void	set_line(t_man *man, int x, t_ray *r);

void	raycasting(t_man *man)
{
	int		x;

	cast_floor(man);
	reset_z_buffer(man);
	sort_sprites_by_dist(man);
	man->rays = 0;
	x = 0;
	while (x < man->frame.size.x)
	{
		perform_dda(man, 2 * x / (double)man->frame.size.x - 1);
		process_dda_list(man, x);
		++x;
	}
	return ;
}

static void	reset_z_buffer(t_man *man)
{
	int	i;

	i = 0;
	while (i < man->frame.size.x)
		man->z_buf[i++] = HUGE_VAL;
	return ;
}

static void	process_dda_list(t_man *man, int x)
{
	t_ray	*r;

	while (man->rays)
	{
		r = (t_ray *)man->rays->data;
		if (r->is_see_through)
		{
			cast_ceiling_x(man, x);
			cast_sprites(man, x);
		}
		if (r->perp_wall_dist < man->z_buf[x])
			man->z_buf[x] = r->perp_wall_dist;
		set_line(man, x, r);
		draw_wall(man, r, r->tex);
		draw_wall(man, r, r->tex_portal);
		list_del_one(&man->rays, free);
	}
	cast_ceiling_x(man, x);
	cast_sprites(man, x);
	return ;
}

static void	set_line(t_man *man, int x, t_ray *r)
{
	t_cell	*cell;
	int		offset;

	cell = &man->maps[man->curr_map]->cells[r->m_index.y][r->m_index.x];
	r->line_height_cubic = man->frame.size.y / r->perp_wall_dist;
	r->line_height = r->line_height_cubic * cell->height;
	offset = r->line_height_cubic * (1.0 - cell->height) * 0.5;
	r->unclamped_line_height.x = -r->line_height / 2 + man->frame.size.y / 2
		+ offset;
	r->unclamped_line_height.y = r->line_height / 2 + man->frame.size.y / 2
		+ offset;
	r->coord1.x = x;
	r->coord1.y = max(r->unclamped_line_height.x, 0);
	r->coord2.x = x;
	r->coord2.y = min(r->unclamped_line_height.y, man->frame.size.y - 1);
	return ;
}
