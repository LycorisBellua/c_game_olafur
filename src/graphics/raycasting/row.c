#include "olafur.h"

static void		init_row(t_man *man, t_row *row);
static int		update_row(t_man *man, t_row *row, int *y);
static t_color	sample_surface(t_map *m, t_row *row, t_img *tex, t_vec2 frac);
static void		draw_fc_pixel(t_man *man, t_row *row, t_ivec2 at, t_map *map);
static void		reset_ceiling_depth(t_man *man);
static void		draw_ceiling_pixel(t_man *man, t_color c, t_ivec2 at, float d);
static t_color	sample_sky(t_png *bg, int off, int sx, int sy);
static void		fill_sky_pair(t_man *man, t_map *map, int y);

void	cast_floor_and_ceiling(t_man *man)
{
	int		x;
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
		{
			x = 0;
			while (x < man->frame.size.x)
			{
				draw_fc_pixel(man, &row, (t_ivec2){x, y}, map);
				row.floor.x += row.floor_step.x;
				row.floor.y += row.floor_step.y;
				++x;
			}
		}
		else
			fill_sky_pair(man, map, y);
		++y;
	}
	fill_sky_pair(man, map, man->frame.size.y / 2);
	return ;
}

static t_color	sample_sky(t_png *bg, int off, int sx, int sy)
{
	int	sc;

	sc = sx + off;
	if (sc >= bg->size.x)
		sc -= bg->size.x;
	return (bg->buf[sy * bg->size.x + sc]);
}

/*
	Fills a mirrored pair of rows (a floor row and its ceiling counterpart) 
	with the skybox across the full width. Used for the beyond-distance rows 
	and for the central horizon rows that the mirrored loop never visits.
*/
static void	fill_sky_pair(t_man *man, t_map *map, int y)
{
	t_png	*bg;
	int		off;
	int		my;
	int		x;

	bg = map->background;
	off = map->background_offset;
	my = man->frame.size.y - y - 1;
	x = 0;
	while (x < man->frame.size.x)
	{
		draw_point_fast(man, sample_sky(bg, off, x, y), x, y);
		draw_point_fast(man, sample_sky(bg, off, x, my), x, my);
		++x;
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

static void	draw_ceiling_pixel(t_man *man, t_color c, t_ivec2 at, float d)
{
	draw_point_fast(man, c, at.x, at.y);
	man->fc_depth[at.y * man->frame.size.x + at.x] = d;
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
	row->pos_z = man->frame.size.y * 0.5;
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
	row->fog_factor8 = (int)(fog_factor_of(row->row_dist, man->dof) * 256.0f);
	return (1);
}

static t_color	sample_surface(t_map *m, t_row *row, t_img *tex, t_vec2 frac)
{
	t_ivec2	coord;
	t_color	color;

	color.a = 0;
	if (!tex)
		return (color);
	coord.x = (int)(tex->size.x * frac.x);
	coord.y = (int)(tex->size.y * frac.y);
	color = tex->cycle[tex->cycle_index][coord.x * tex->size.y + coord.y];
	apply_fog_factor(&color, m->fog_color, row->fog_factor8);
	return (color);
}

static void	draw_fc_pixel(t_man *man, t_row *row, t_ivec2 at, t_map *map)
{
	t_ivec2	cell;
	t_vec2	frac;
	t_cell	*cc;
	t_color	color;
	int		my;

	my = man->frame.size.y - at.y - 1;
	if (row->floor.x < 0 || row->floor.x >= map->size.x
		|| row->floor.y < 0 || row->floor.y >= map->size.y)
	{
		draw_point_fast(man, sample_sky(map->background,
				map->background_offset, at.x, at.y), at.x, at.y);
		draw_point_fast(man, sample_sky(map->background,
				map->background_offset, at.x, my), at.x, my);
		return ;
	}
	cell.x = (int)row->floor.x;
	cell.y = (int)row->floor.y;
	frac.x = row->floor.x - cell.x;
	frac.y = row->floor.y - cell.y;
	cc = &map->cells[cell.y][cell.x];
	color = sample_surface(map, row, cc->tex_floor, frac);
	if (color.a != 255)
		draw_point_fast(man, sample_sky(map->background,
				map->background_offset, at.x, at.y), at.x, at.y);
	if (color.a)
		draw_point_fast(man, color, at.x, at.y);
	color = sample_surface(map, row, cc->tex_ceiling, frac);
	if (color.a != 255)
		draw_point_fast(man, sample_sky(map->background,
				map->background_offset, at.x, my), at.x, my);
	if (color.a)
		draw_ceiling_pixel(man, color, (t_ivec2){at.x, my}, row->row_dist);
}
