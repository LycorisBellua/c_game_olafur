#include "cimmerian.h"

static void		init_row(t_man *man, t_row *row);
static int		update_row(t_man *man, t_row *row, int *y);
static t_color	calculate_color(t_man *man, t_map *m, t_row *row, int is_floor);

void	cast_floor(t_man *man)
{
	int		x;
	int		y;
	t_row	row;
	t_color	color;

	init_row(man, &row);
	y = man->frame.size.y / 2 + 1;
	while (y < man->frame.size.y)
	{
		if (!update_row(man, &row, &y))
			continue ;
		x = 0;
		while (x < man->frame.size.x)
		{
			color = calculate_color(man, man->maps[man->curr_map], &row, 1);
			if (color.a)
				draw_point(man, color, x, y);
			row.floor.x += row.floor_step.x;
			row.floor.y += row.floor_step.y;
			++x;
		}
		++y;
	}
	return ;
}

void	cast_ceiling_x(t_man *man, int x)
{
	int		y;
	t_row	row;
	t_color	color;

	init_row(man, &row);
	y = man->frame.size.y / 2 + 1;
	while (y < man->frame.size.y)
	{
		if (!update_row(man, &row, &y))
			continue ;
		row.floor.x += x * row.floor_step.x;
		row.floor.y += x * row.floor_step.y;
		if (row.row_dist < man->z_buf[x])
		{
			color = calculate_color(man, man->maps[man->curr_map], &row, 0);
			if (color.a)
				draw_point(man, color, x, man->frame.size.y - y - 1);
		}
		++y;
	}
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
	row->pos_z = 0.5 * man->res.h_mod * man->frame.size.y;
	return ;
}

static int	update_row(t_man *man, t_row *row, int *y)
{
	row->row_dist = row->pos_z / (*y - man->frame.size.y / 2 + 1);
	if (row->row_dist > man->dof)
	{
		++*y;
		return (0);
	}
	row->floor_step.x = row->row_dist * row->ray_dir_step.x;
	row->floor_step.y = row->row_dist * row->ray_dir_step.y;
	row->floor.x = man->player.pos.x + row->row_dist * row->ray_dir.x;
	row->floor.y = man->player.pos.y + row->row_dist * row->ray_dir.y;
	return (1);
}

static t_color	calculate_color(t_man *man, t_map *m, t_row *row, int is_floor)
{
	t_ivec2	cell;
	t_img	*tex;
	t_ivec2	coord;
	t_color	color;

	color.a = 0;
	if (row->floor.x < 0 || row->floor.x >= m->size.x
		|| row->floor.y < 0 || row->floor.y >= m->size.y)
		return (color);
	cell.x = (int)row->floor.x;
	cell.y = (int)row->floor.y;
	if (is_floor)
		tex = m->cells[cell.y][cell.x].tex_floor;
	else
		tex = m->cells[cell.y][cell.x].tex_ceiling;
	if (!tex)
		return (color);
	coord.x = (int)(tex->size.x * (row->floor.x - cell.x)) % tex->size.x;
	coord.y = (int)(tex->size.y * (row->floor.y - cell.y)) % tex->size.y;
	color = tex->cycle[tex->cycle_index][tex->size.x * coord.y + coord.x];
	color.r *= 0.7;
	color.g *= 0.7;
	color.b *= 0.7;
	apply_wall_fog(&color, m->fog_color, row->row_dist, man->dof);
	return (color);
}
