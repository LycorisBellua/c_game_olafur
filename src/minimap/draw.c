#include "cimmerian.h"

static t_vert	get_rect_vert(t_man *man, t_ivec2 i_map, t_ivec2 i_cell);
static void		draw_player(t_man *man);
static void		draw_bubble(t_man *man);

void	draw_minimap(t_man *man, t_map *map)
{
	t_ivec2	i_map;
	t_ivec2	i_cell;

	i_cell.y = 0;
	i_map.y = (int)man->player.pos.y - man->minimap_cell_amount * 0.5;
	while (i_map.y < (int)man->player.pos.y + man->minimap_cell_amount
		* 0.5 + 1)
	{
		i_cell.x = 0;
		i_map.x = (int)man->player.pos.x - man->minimap_cell_amount * 0.5;
		while (i_map.x < (int)man->player.pos.x + man->minimap_cell_amount
			* 0.5 + 1)
		{
			if (i_map.x >= 0 && i_map.y >= 0 && i_map.x < map->size.x
				&& i_map.y < map->size.y)
				draw_rect_minimap(man, get_rect_vert(man, i_map, i_cell));
			++i_map.x;
			++i_cell.x;
		}
		++i_map.y;
		++i_cell.y;
	}
	draw_player(man);
	draw_bubble(man);
	return ;
}

// Enemy cell color: rgba(231, 111, 81, 255)
static t_vert	get_rect_vert(t_man *man, t_ivec2 i_map, t_ivec2 i_cell)
{
	t_vert	v;
	t_cell	*c;

	v.coord.x = man->minimap_offset.x + (i_cell.x + ((int)man->player.pos.x
				- man->player.pos.x)) * man->minimap_zoom;
	v.coord.y = man->minimap_offset.y + (i_cell.y + ((int)man->player.pos.y
				- man->player.pos.y)) * man->minimap_zoom;
	v.color = get_color_rgba(0, 0, 0, 0);
	c = &man->maps[man->curr_map]->cells[i_map.y][i_map.x];
	if (c->is_goal)
		v.color = get_color_rgba(28, 210, 192, 255);
	else if ((c->door || c->portal) && c->is_visible)
		v.color = get_color_rgba(244, 162, 97, 255);
	else if (c->is_visible)
		v.color = get_color_rgba(40, 114, 113, 255);
	else if (!c->is_empty_space)
		v.color = get_color_rgba(38, 70, 83, 255);
	return (v);
}

static void	draw_player(t_man *man)
{
	t_vert	center;

	set_ivec2(&center.coord,
		man->minimap_center.x - 1,
		man->minimap_center.y - 1);
	center.color = get_color_rgba(233, 196, 106, 255);
	draw_circle_full(man, center, man->minimap_zoom * 0.25);
	return ;
}

static void	draw_bubble(t_man *man)
{
	t_vert	center;

	set_ivec2(&center.coord, man->minimap_center.x, man->minimap_center.y);
	center.color = get_color_rgba(255 * 0.25, 255 * 0.25, 255 * 0.25,
			255 * 0.25);
	draw_circle_full_gradient(man, center, man->minimap_radius,
		man->maps[man->curr_map]->fog_color);
	center.color = get_color_rgba(38, 70, 83, 255);
	draw_circle(man, center, man->minimap_radius);
	return ;
}
