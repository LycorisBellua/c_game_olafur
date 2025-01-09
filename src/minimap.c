#include "cimmerian.h"

static void draw_rectangle_minimap(t_frame *f, t_vert v, t_ivec2 size,
	t_ivec2 circle_center, int circle_radius)
{
	t_ivec2	max;
	t_ivec2	point;
	t_ivec2	delta;
	int		circle_radius_sq;

	circle_radius_sq = circle_radius * circle_radius;
	max.x = v.coord.x + size.x - 1;
	max.y = v.coord.y + size.y - 1;
	point.y = v.coord.y - 1;
	while (++point.y <= max.y)
	{
		point.x = v.coord.x - 1;
		while (++point.x <= max.x)
		{
			delta.x = point.x - circle_center.x;
			delta.y = point.y - circle_center.y;
			if (delta.x * delta.x + delta.y * delta.y <= circle_radius_sq)
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

static void	draw_player(t_frame *f, t_ivec2 map_offset, t_ivec2 cell_amount,
	t_ivec2 cell_size)
{
	t_vert	center;

	set_ivec2(&center.coord, map_offset.x + cell_amount.x / 2 * cell_size.x - 1,
		map_offset.y + cell_amount.y / 2 * cell_size.y - 1);
	center.color = get_color_rgba(233, 196, 106, 255);
	draw_circle_full(f, center, min(cell_size.x, cell_size.y) * 0.25);
	return ;
}

static void	draw_bubble(t_frame *f, t_ivec2 map_offset, t_ivec2 cell_amount,
	t_ivec2 cell_size, t_color fog)
{
	t_vert	center;
	int		radius;

	set_ivec2(&center.coord, map_offset.x + cell_amount.x / 2 * cell_size.x,
		map_offset.y + cell_amount.y / 2 * cell_size.y);
	center.color = get_color_rgba(255 * 0.5, 255 * 0.5, 255 * 0.5, 255 * 0.25);
	radius = f_max(cell_amount.x / 2 * cell_size.x,
		cell_amount.y / 2 * cell_size.y);
	draw_circle_full_gradient(f, center, radius, fog);
	center.color = get_color_rgba(38, 70, 83, 255); //empty cell
	draw_circle(f, center, radius);
	return ;
}

void	draw_minimap(t_frame *f, t_map *m)
{
	t_ivec2 cell_size;
	t_ivec2	cell_amount;
	t_ivec2	map_padding;
	t_ivec2	map_offset;
	t_ivec2	i_map;
	t_ivec2	i_cell;
	t_vert	v;

	set_ivec2(&cell_size, 9, 9);
	set_ivec2(&cell_amount, 8, 8);
	set_ivec2(&map_padding, 5, 5);
	set_ivec2(&map_offset,
		f->size.x - cell_amount.x * cell_size.x - map_padding.x,
		map_padding.y);

	t_ivec2	circle_center;
	int		circle_radius;

	set_ivec2(&circle_center, map_offset.x + cell_amount.x / 2 * cell_size.x,
		map_offset.y + cell_amount.y / 2 * cell_size.y);
	circle_radius = f_max(cell_amount.x / 2 * cell_size.x,
		cell_amount.y / 2 * cell_size.y);


	i_cell.y = 0;
	i_map.y = (int)g_man.player.pos.y - cell_amount.y / 2;
	while (i_map.y < (int)g_man.player.pos.y + cell_amount.y / 2 + 1)
	{
		i_cell.x = 0;
		i_map.x = (int)g_man.player.pos.x - cell_amount.x / 2;
		while (i_map.x < (int)g_man.player.pos.x + cell_amount.x / 2 + 1)
		{
			if (i_map.x >= 0 && i_map.y >= 0 && i_map.x < m->size.x && i_map.y < m->size.y)
			{
				v.coord.x = map_offset.x + (i_cell.x + ((int)g_man.player.pos.x - g_man.player.pos.x)) * cell_size.x;
				v.coord.y = map_offset.y + (i_cell.y + ((int)g_man.player.pos.y - g_man.player.pos.y)) * cell_size.y;
				v.color = get_cell_color(m, i_map);
				set_ivec2(&cell_size, 9 - 1, 9 - 1);

				// RIGHT
				if (i_cell.x == 8)
				{
					double diff = g_man.player.pos.x - (int)g_man.player.pos.x;
					cell_size.x = 9 * diff + 1;
				}
				// LEFT
				if (i_cell.x == 0)
				{
					double diff = g_man.player.pos.x - (int)g_man.player.pos.x;
					cell_size.x = 9 - 9 * diff - 1;
					if (diff == 0)
						--cell_size.x;
					v.coord.x += 9 * diff + 1;
				}
				// TOP
				if (i_cell.y == 0)
				{
					double diff = g_man.player.pos.y - (int)g_man.player.pos.y;
					cell_size.y = 9 - 9 * diff - 2;
					v.coord.y += 9 * diff + 1;
					if (cell_size.y > 1 && cell_size.y < 7)
						++v.coord.y;
				}
				// BOTTOM
				if (i_cell.y == 8)
				{
					double diff = g_man.player.pos.y - (int)g_man.player.pos.y;
					cell_size.y = 9 * diff;
					if (diff > 0 && cell_size.y < 8)
						++cell_size.y;
				}

				if (cell_size.x > 0 && cell_size.y > 0)
					draw_rectangle_minimap(f, v, cell_size, circle_center, circle_radius);
				set_ivec2(&cell_size, 9, 9);
			}
			++i_map.x;
			++i_cell.x;
		}
		++i_map.y;
		++i_cell.y;
	}

	draw_player(f, map_offset, cell_amount, cell_size);
	draw_bubble(f, map_offset, cell_amount, cell_size, m->fog_color);
	return ;
}
