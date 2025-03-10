#include "cimmerian.h"

static void	set_line(t_frame *f, t_map *m, int x, t_ray *r);

void	raycasting(t_frame *f, t_map *m)
{
	int		x;
	t_list	*list;
	double	*z_buffer;
	int		spr_order[NBR_SPR];
	double	spr_dist[NBR_SPR];

	cast_floor(f, m);

	z_buffer = malloc(f->size.x * sizeof(double));
	int i = 0;
	while (i < f->size.x)
		z_buffer[i++] = LLONG_MAX;

	sort_sprites(spr_order, spr_dist, NBR_SPR);

	list = 0;
	x = 0;
	while (x < f->size.x)
	{
		perform_dda(m, 2 * x / (double)f->size.x - 1, &list);
		while (list)
		{
			t_ray *r = (t_ray *)list->data;
			if (r->is_see_through)
			{
				cast_ceiling_x(f, m, z_buffer, x);
				cast_sprites(f, m, z_buffer, spr_order, spr_dist, x);
			}
			if (r->perp_wall_dist < z_buffer[x])
				z_buffer[x] = r->perp_wall_dist;
			set_line(f, m, x, r);
			draw_wall(f, m, r);
			list_del_one(&list, free);
		}
		cast_ceiling_x(f, m, z_buffer, x);
		cast_sprites(f, m, z_buffer, spr_order, spr_dist, x);
		++x;
	}
	free(z_buffer);
	return ;
}

static void	set_line(t_frame *f, t_map *m, int x, t_ray *r)
{
	t_cell	*cell;
	int		offset;

	cell = &m->cells[r->m_index.y * m->size.x + r->m_index.x];
	r->line_height_cubic = (int)(f->size.y / r->perp_wall_dist * g_man.res.h_mod);
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
