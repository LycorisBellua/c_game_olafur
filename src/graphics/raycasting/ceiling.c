#include "cimmerian.h"

void	cast_ceiling_x(t_frame *f, t_map *m, double *z_buffer, int x)
{
	int		y;
	t_vec2	ray_dir0;
	t_vec2	ray_dir1;
	t_vec2	ray_dir_step;
	double	pos_z;
	int		p;
	double	row_dist;
	t_vec2	floor_step;
	t_vec2	floor;
	t_ivec2	cell;
	t_img	*tex;
	t_ivec2	t;
	t_color	color;

	ray_dir0.x = g_man.player.dir.x - g_man.player.plane.x;
	ray_dir0.y = g_man.player.dir.y - g_man.player.plane.y;
	ray_dir1.x = g_man.player.dir.x + g_man.player.plane.x;
	ray_dir1.y = g_man.player.dir.y + g_man.player.plane.y;
	ray_dir_step.x = (ray_dir1.x - ray_dir0.x) / f->size.x;
	ray_dir_step.y = (ray_dir1.y - ray_dir0.y) / f->size.x;
	pos_z = 0.5 * g_man.res.h_mod * f->size.y;

	y = f->size.y / 2 + 1;
	while (y < f->size.y)
	{
		p = y - f->size.y / 2 + 1;
		row_dist = pos_z / p;
		if (row_dist > m->dof)
		{
			++y;
			continue ;
		}
		floor_step.x = row_dist * ray_dir_step.x;
		floor_step.y = row_dist * ray_dir_step.y;
		floor.x = g_man.player.pos.x + row_dist * ray_dir0.x;
		floor.y = g_man.player.pos.y + row_dist * ray_dir0.y;

		floor.x += x * floor_step.x;
		floor.y += x * floor_step.y;
		cell.x = (int)floor.x;
		cell.y = (int)floor.y;
		if (cell.x >= 0 && cell.x < m->size.x && cell.y >= 0
				&& cell.y < m->size.y && row_dist < z_buffer[x])
		{
			tex = m->cells[cell.y * m->size.x + cell.x].tex_ceiling;
			if (tex)
			{
				t.x = (int)(tex->size.x * (floor.x - cell.x)) % tex->size.x;
				t.y = (int)(tex->size.y * (floor.y - cell.y)) % tex->size.y;
				color = ((t_color *)tex->buf)[tex->size.x * t.y + t.x];
				color.r /= 2;
				color.g /= 2;
				color.b /= 2;
				apply_wall_fog(&color, m->fog_color, row_dist, m->dof);
				draw_point(f, color, x, f->size.y - y - 1);
			}
		}
		++y;
	}
	return ;
}
