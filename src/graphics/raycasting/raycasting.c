#include "cimmerian.h"

static void	set_line(t_man *man, t_frame *f, int x, t_ray *r);

void	raycasting(t_man *man, t_frame *f)
{
	int		x;
	t_list	*list;
	double	*z_buffer;
	int		spr_order[NBR_SPR];
	double	spr_dist[NBR_SPR];

	cast_floor(man, f);

	z_buffer = malloc(f->size.x * sizeof(double));
	int i = 0;
	while (i < f->size.x)
		z_buffer[i++] = INF;

	sort_sprites(man, spr_order, spr_dist, NBR_SPR);

	list = 0;
	x = 0;
	while (x < f->size.x)
	{
		perform_dda(man, 2 * x / (double)f->size.x - 1, &list);
		while (list)
		{
			t_ray *r = (t_ray *)list->data;
			if (r->is_see_through)
			{
				cast_ceiling_x(man, f, z_buffer, x);
				cast_sprites(man, f, z_buffer, spr_order, spr_dist, x);
			}
			if (r->perp_wall_dist < z_buffer[x])
				z_buffer[x] = r->perp_wall_dist;
			set_line(man, f, x, r);
			draw_wall(man, f, r);
			list_del_one(&list, free);
		}
		cast_ceiling_x(man, f, z_buffer, x);
		cast_sprites(man, f, z_buffer, spr_order, spr_dist, x);
		++x;
	}
	free(z_buffer);
	return ;
}

static void	set_line(t_man *man, t_frame *f, int x, t_ray *r)
{
	t_cell	*cell;
	int		offset;

	cell = &man->map->cells[r->m_index.y * man->map->size.x + r->m_index.x];
	r->line_height_cubic = (int)(f->size.y / r->perp_wall_dist * man->res.h_mod);
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
