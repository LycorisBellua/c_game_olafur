#include "cimmerian.h"

static void	reset_z_buffer(t_man *man, t_frame *f);
static void	process_dda_list(t_man *man, t_list **list, int x);
static void	set_line(t_man *man, t_frame *f, int x, t_ray *r);

void	raycasting(t_man *man, t_frame *f)
{
	int		x;
	t_list	*list;

	cast_floor(man, f);
	reset_z_buffer(man, f);
	sort_sprites_by_dist(man);
	list = 0;
	x = 0;
	while (x < f->size.x)
	{
		perform_dda(man, 2 * x / (double)f->size.x - 1, &list);
		process_dda_list(man, &list, x);
		++x;
	}
	return ;
}

static void	reset_z_buffer(t_man *man, t_frame *f)
{
	int	i;

	i = 0;
	while (i < f->size.x)
		man->z_buf[i++] = HUGE_VAL;
	return ;
}

static void	process_dda_list(t_man *man, t_list **list, int x)
{
	t_frame	*f;
	t_ray	*r;

	f = man->frame + man->curr_frame;
	while (*list)
	{
		r = (t_ray *)(*list)->data;
		if (r->is_see_through)
		{
			cast_ceiling_x(man, f, x);
			cast_sprites(man, x);
		}
		if (r->perp_wall_dist < man->z_buf[x])
			man->z_buf[x] = r->perp_wall_dist;
		set_line(man, f, x, r);
		draw_wall(man, f, r, 0);
		draw_wall(man, f, r, 1);
		list_del_one(list, free);
	}
	cast_ceiling_x(man, f, x);
	cast_sprites(man, x);
	return ;
}

static void	set_line(t_man *man, t_frame *f, int x, t_ray *r)
{
	t_cell	*cell;
	int		offset;

	cell = &man->maps[man->curr_map]->cells[r->m_index.y][r->m_index.x];
	r->line_height_cubic = f->size.y / r->perp_wall_dist * man->res.h_mod;
	r->line_height = r->line_height_cubic * cell->height;
	offset = r->line_height_cubic * (1.0 - cell->height) * 0.5;
	r->unclamped_line_height.x = -r->line_height / 2 + f->size.y / 2 + offset;
	r->unclamped_line_height.y = r->line_height / 2 + f->size.y / 2 + offset;
	r->coord1.x = x;
	r->coord1.y = max(r->unclamped_line_height.x, 0);
	r->coord2.x = x;
	r->coord2.y = min(r->unclamped_line_height.y, f->size.y - 1);
	return ;
}
