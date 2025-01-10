#include "cimmerian.h"

static void draw_rectangle_minimap(t_frame *f, t_map *m, t_vert v)
{
	t_ivec2	max;
	t_ivec2	point;
	t_ivec2	delta;
	t_ivec2	size;
	int		circle_radius_sq;

	set_ivec2(&size, m->minimap_zoom - 1, m->minimap_zoom - 1);
	if (!size.x)
		set_ivec2(&size, 1, 1);
	//if (v.coord.x <= 0)
	//	--size.x;
	if (v.coord.y <= 0)
		--size.y;
	circle_radius_sq = m->minimap_radius * m->minimap_radius;
	max.x = v.coord.x + size.x - 1;
	max.y = v.coord.y + size.y - 1;
	point.y = v.coord.y - 1;
	while (++point.y <= max.y)
	{
		point.x = v.coord.x - 1;
		while (++point.x <= max.x)
		{
			delta.x = point.x - m->minimap_center.x;
			delta.y = point.y - m->minimap_center.y;
			if (delta.x * delta.x + delta.y * delta.y <= circle_radius_sq + m->minimap_zoom)
				draw_point(f, v.color, point.x, point.y);
		}
	}
	return ;
}

static t_color	get_cell_color(t_map *m, t_ivec2 i_map)
{
	if (m->cells[i_map.y * m->size.x + i_map.x].is_goal)
		return (get_color_rgba(28, 210, 192, 255)); //obelisk cell
	else if (m->cells[i_map.y * m->size.x + i_map.x].is_door)
		return (get_color_rgba(244, 162, 97, 255)); //door cell
	else if (m->cells[i_map.y * m->size.x + i_map.x].is_obstacle)
		return (get_color_rgba(40, 114, 113, 255)); //wall cell
	//get_color_rgba(231, 111, 81, 255); //enemy cell
	return (get_color_rgba(38, 70, 83, 255)); //empty cell
}

static void	draw_player(t_frame *f, t_map *m)
{
	t_vert	center;

	set_ivec2(&center.coord, m->minimap_center.x - 1, m->minimap_center.y - 1);
	center.color = get_color_rgba(233, 196, 106, 255);
	draw_circle_full(f, center, m->minimap_zoom * 0.25);
	return ;
}

static void	draw_bubble(t_frame *f, t_map *m)
{
	t_vert	center;

	set_ivec2(&center.coord, m->minimap_center.x, m->minimap_center.y);
	center.color = get_color_rgba(255 * 0.25, 255 * 0.25, 255 * 0.25, 255 * 0.25);
	draw_circle_full_gradient(f, center, m->minimap_radius, m->fog_color);
	center.color = get_color_rgba(38, 70, 83, 255); //empty cell
	draw_circle(f, center, m->minimap_radius);
	return ;
}

void	draw_minimap(t_frame *f, t_map *m)
{
	t_ivec2	i_map;
	t_ivec2	i_cell;
	t_vert	v;

	//
	set_ivec2(&m->minimap_offset, 563 - 563, 5);
	set_ivec2(&m->minimap_center, 599 - 563, 41);
	m->minimap_radius = 36;
	m->minimap_zoom = 9; //1, 2, 3, 4, 6, 9, 12, 18
	//

	int cell_amount = m->minimap_radius / m->minimap_zoom * 2;

	i_cell.y = 0;
	i_map.y = (int)g_man.player.pos.y - cell_amount / 2;
	while (i_map.y < (int)g_man.player.pos.y + cell_amount / 2 + 1)
	{
		i_cell.x = 0;
		i_map.x = (int)g_man.player.pos.x - cell_amount / 2;
		while (i_map.x < (int)g_man.player.pos.x + cell_amount / 2 + 1)
		{
			if (i_map.x >= 0 && i_map.y >= 0 && i_map.x < m->size.x && i_map.y < m->size.y)
			{
				v.coord.x = m->minimap_offset.x + (i_cell.x + ((int)g_man.player.pos.x - g_man.player.pos.x)) * m->minimap_zoom;
				v.coord.y = m->minimap_offset.y + (i_cell.y + ((int)g_man.player.pos.y - g_man.player.pos.y)) * m->minimap_zoom;
				v.color = get_cell_color(m, i_map);
				draw_rectangle_minimap(f, m, v);
			}
			++i_map.x;
			++i_cell.x;
		}
		++i_map.y;
		++i_cell.y;
	}

	draw_player(f, m);
	draw_bubble(f, m);
	return ;
}
