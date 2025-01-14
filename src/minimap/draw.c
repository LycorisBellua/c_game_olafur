#include "cimmerian.h"

static t_vert	get_rect_vert(t_map *m, t_ivec2 i_map, t_ivec2 i_cell);
static void		draw_rectangle_minimap(t_frame *f, t_map *m, t_vert v);
static void		draw_player(t_frame *f, t_map *m);
static void		draw_bubble(t_frame *f, t_map *m);

void	draw_minimap(t_frame *f, t_map *m)
{
	t_ivec2	i_map;
	t_ivec2	i_cell;

	i_cell.y = 0;
	i_map.y = (int)g_man.player.pos.y - m->minimap_cell_amount * 0.5;
	while (i_map.y < (int)g_man.player.pos.y + m->minimap_cell_amount * 0.5 + 1)
	{
		i_cell.x = 0;
		i_map.x = (int)g_man.player.pos.x - m->minimap_cell_amount * 0.5;
		while (i_map.x < (int)g_man.player.pos.x + m->minimap_cell_amount * 0.5
			+ 1)
		{
			if (i_map.x >= 0 && i_map.y >= 0 && i_map.x < m->size.x
				&& i_map.y < m->size.y)
				draw_rectangle_minimap(f, m, get_rect_vert(m, i_map, i_cell));
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

// Enemy cell color: rgba(231, 111, 81, 255)
static t_vert	get_rect_vert(t_map *m, t_ivec2 i_map, t_ivec2 i_cell)
{
	t_vert	v;

	v.coord.x = m->minimap_offset.x + (i_cell.x + ((int)g_man.player.pos.x
				- g_man.player.pos.x)) * m->minimap_zoom;
	v.coord.y = m->minimap_offset.y + (i_cell.y + ((int)g_man.player.pos.y
				- g_man.player.pos.y)) * m->minimap_zoom;
	if (m->cells[i_map.y * m->size.x + i_map.x].is_goal)
		v.color = get_color_rgba(28, 210, 192, 255);
	else if (m->cells[i_map.y * m->size.x + i_map.x].is_door)
		v.color = get_color_rgba(244, 162, 97, 255);
	else if (m->cells[i_map.y * m->size.x + i_map.x].is_obstacle)
		v.color = get_color_rgba(40, 114, 113, 255);
	else
		v.color = get_color_rgba(38, 70, 83, 255);
	return (v);
}

static void	draw_rectangle_minimap(t_frame *f, t_map *m, t_vert v)
{
	t_ivec2	point;
	t_ivec2	delta;
	t_ivec2	size;
	int		circle_radius_sq;

	set_ivec2(&size, m->minimap_zoom - 1, m->minimap_zoom - 1);
	if (!size.x)
		set_ivec2(&size, 1, 1);
	if (v.coord.y <= 0)
		--size.y;
	circle_radius_sq = m->minimap_radius * m->minimap_radius;
	point.y = v.coord.y - 1;
	while (++point.y <= v.coord.y + size.y - 1)
	{
		point.x = v.coord.x - 1;
		while (++point.x <= v.coord.x + size.x - 1)
		{
			delta.x = point.x - m->minimap_center.x;
			delta.y = point.y - m->minimap_center.y;
			if (delta.x * delta.x + delta.y * delta.y <= circle_radius_sq
				+ m->minimap_zoom)
				draw_point(f, v.color, point.x, point.y);
		}
	}
	return ;
}

static void	draw_player(t_frame *f, t_map *m)
{
	t_vert	center;
	t_vert	v2;

	set_ivec2(&center.coord, m->minimap_center.x - 1, m->minimap_center.y - 1);
	center.color = get_color_rgba(233, 196, 106, 255);
	draw_circle_full(f, center, m->minimap_zoom * 0.25);
	center.color = get_color_rgba(255 * 0.75, 255 * 0.75, 255 * 0.75, 255);
	center.coord.x += 1 * g_man.player.dir.x;
	center.coord.y += 1 * g_man.player.dir.y;
	set_ivec2(&v2.coord, center.coord.x + m->minimap_zoom * g_man.player.dir.x,
		center.coord.y + m->minimap_zoom * g_man.player.dir.y);
	v2.color = center.color;
	draw_line(f, center, v2);
	return ;
}

static void	draw_bubble(t_frame *f, t_map *m)
{
	t_vert	center;

	set_ivec2(&center.coord, m->minimap_center.x, m->minimap_center.y);
	center.color = get_color_rgba(255 * 0.25, 255 * 0.25, 255 * 0.25,
			255 * 0.25);
	draw_circle_full_gradient(f, center, m->minimap_radius, m->fog_color);
	center.color = get_color_rgba(38, 70, 83, 255);
	draw_circle(f, center, m->minimap_radius);
	return ;
}
