#include "olafur.h"

static void	draw_row(t_man *man, t_row *row, t_map *map, int y);
static void	reset_ceiling_depth(t_man *man);
static void	init_row(t_man *man, t_row *row);
static int	update_row(t_man *man, t_row *row, int *y);

void	cast_floor_and_ceiling(t_man *man)
{
	int		y;
	t_row	row;
	t_map	*map;

	map = man->maps[man->curr_map];
	reset_ceiling_depth(man);
	init_row(man, &row);
	y = man->frame.size.y / 2 + 1;
	while (y < man->frame.size.y)
	{
		if (update_row(man, &row, &y))
			draw_row(man, &row, map, y);
		else
			fill_sky_pair(man, map, y);
		++y;
	}
	fill_sky_pair(man, map, man->frame.size.y / 2);
	return ;
}

static void	draw_row(t_man *man, t_row *row, t_map *map, int y)
{
	t_ivec2	at;

	set_ivec2(&at, 0, y);
	while (at.x < man->frame.size.x)
	{
		row->fog_factor8 = imin(row->fog_scale * man->ray_len[at.x], 256);
		draw_fc_pixel(man, row, at, map);
		row->floor.x += row->floor_step.x;
		row->floor.y += row->floor_step.y;
		++at.x;
	}
	return ;
}

static void	reset_ceiling_depth(t_man *man)
{
	int	i;
	int	n;

	i = 0;
	n = man->frame.size.y / 2 * man->frame.size.x;
	while (i < n)
		man->fc_depth[i++] = HUGE_VALF;
	return ;
}

static void	init_row(t_man *man, t_row *row)
{
	t_vec2	tmp_ray_dir;

	row->ray_dir.x = man->player.dir.x - man->player.plane.x;
	row->ray_dir.y = man->player.dir.y - man->player.plane.y;
	tmp_ray_dir.x = man->player.dir.x + man->player.plane.x;
	tmp_ray_dir.y = man->player.dir.y + man->player.plane.y;
	row->ray_dir_step.x = (tmp_ray_dir.x - row->ray_dir.x) / man->frame.size.x;
	row->ray_dir_step.y = (tmp_ray_dir.y - row->ray_dir.y) / man->frame.size.x;
	row->pos_z = man->frame.size.y * 0.5f;
	return ;
}

static int	update_row(t_man *man, t_row *row, int *y)
{
	row->row_dist = row->pos_z / (*y - man->frame.size.y / 2 + 1);
	if (row->row_dist > man->dof)
		return (0);
	row->floor_step.x = row->row_dist * row->ray_dir_step.x;
	row->floor_step.y = row->row_dist * row->ray_dir_step.y;
	row->floor.x = man->player.pos.x + row->row_dist * row->ray_dir.x;
	row->floor.y = man->player.pos.y + row->row_dist * row->ray_dir.y;
	row->fog_scale = row->row_dist / man->dof * 256.0f;
	return (1);
}
