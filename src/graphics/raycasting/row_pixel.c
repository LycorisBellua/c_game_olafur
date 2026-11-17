#include "olafur.h"

static void		draw_sky_pixel(t_man *man, t_map *map, int x, int y);
static void		draw_ceiling_pixel(t_man *man, t_color c, t_ivec2 at, float d);
static t_color	sample_surface(t_map *m, t_row *row, int ceiling);

/*
	Draws a floor pixel and its mirrored ceiling pixel. Outside of the map, or 
	where a surface is missing or translucent, the skybox shows through.
*/
void	draw_fc_pixel(t_man *man, t_row *row, t_ivec2 at, t_map *map)
{
	t_color	color;
	t_ivec2	coord;

	set_ivec2(&coord, at.x, man->frame.size.y - at.y - 1);
	color = sample_surface(map, row, 0);
	if (color.a != 255)
		draw_sky_pixel(man, map, at.x, at.y);
	if (color.a)
		draw_point_fast(man, color, at.x, at.y);
	color = sample_surface(map, row, 1);
	if (color.a != 255)
		draw_sky_pixel(man, map, at.x, coord.y);
	if (color.a)
		draw_ceiling_pixel(man, color, coord, row->row_dist);
	return ;
}

/*
	Fills a mirrored pair of rows (a floor row and its ceiling counterpart) 
	with the skybox across the full width. Used for the beyond-distance rows 
	and for the central horizon rows that the mirrored loop never visits.
*/
void	fill_sky_pair(t_man *man, t_map *map, int y)
{
	int	my;
	int	x;

	my = man->frame.size.y - y - 1;
	x = 0;
	while (x < man->frame.size.x)
	{
		draw_sky_pixel(man, map, x, y);
		draw_sky_pixel(man, map, x, my);
		++x;
	}
	return ;
}

static void	draw_sky_pixel(t_man *man, t_map *map, int x, int y)
{
	t_png	*bg;
	int		sc;

	bg = map->background;
	sc = x + map->background_offset;
	if (sc >= bg->size.x)
		sc -= bg->size.x;
	draw_point_fast(man, bg->buf[y * bg->size.x + sc], x, y);
	return ;
}

static void	draw_ceiling_pixel(t_man *man, t_color c, t_ivec2 at, float d)
{
	draw_point_fast(man, c, at.x, at.y);
	man->fc_depth[at.y * man->frame.size.x + at.x] = d;
	return ;
}

/* Returns a transparent color outside of the map or without a texture. */
static t_color	sample_surface(t_map *m, t_row *row, int ceiling)
{
	t_cell	*cell;
	t_img	*tex;
	t_ivec2	coord;
	t_color	color;

	color.a = 0;
	if (row->floor.x < 0 || row->floor.x >= m->size.x
		|| row->floor.y < 0 || row->floor.y >= m->size.y)
		return (color);
	cell = &m->cells[(int)row->floor.y][(int)row->floor.x];
	tex = cell->tex_floor;
	if (ceiling)
		tex = cell->tex_ceiling;
	if (!tex)
		return (color);
	coord.x = (int)(tex->size.x * (row->floor.x - (int)row->floor.x));
	coord.y = (int)(tex->size.y * (row->floor.y - (int)row->floor.y));
	color = tex->cycle[tex->cycle_index][coord.x * tex->size.y + coord.y];
	apply_fog_factor(&color, m->fog_color, row->fog_factor8);
	return (color);
}
