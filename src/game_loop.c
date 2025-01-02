#include "cimmerian.h"

static void	display_gui(t_frame *f, t_map *m);
static void	draw_minimap(t_frame *f, t_map *m);

void	run_game_loop(t_map *m)
{
	t_frame	*f;

	f = g_man.frame[g_man.curr_frame];
	draw_background(f, m);
	raycasting(f, m);
	door_routine(m);
	display_gui(f, m);
	return ;
}

static void	display_gui(t_frame *f, t_map *m)
{
	t_ivec2	pos;

	draw_minimap(f, m);
	set_ivec2(&pos, 0, 0);
	draw_sprite(f, g_man.sprites + 2, pos, g_man.dt_ms);
	pos.x += g_man.sprites[2].size.x;
	pos.y += 14;
	display_fps(f, pos);
	if (g_man.cursor.x >= 0 && g_man.cursor.y >= 0)
		draw_cursor(f, &g_man.sprites[0], g_man.cursor, g_man.l_click_action);
	return ;
}

static void	draw_minimap(t_frame *f, t_map *m)
{
	t_ivec2	coord;
	t_ivec2	offset;
	t_ivec2	padding;
	t_ivec2 cell_size;
	t_vert	v;

	/*
		8x8 display
	*/

	set_ivec2(&cell_size, 9, 9);
	set_ivec2(&padding, 5, 5);
	set_ivec2(&offset, f->size.x - 8 * cell_size.x - padding.x,
		padding.y);

	// Debug - Border
	v.coord.x = offset.x;
	v.coord.y = offset.y;
	v.color = get_color_rgba(255, 0, 0, 255);
	t_ivec2 size_debug;
	size_debug.x = 8 * cell_size.x;
	size_debug.y = 8 * cell_size.y;
	draw_rectangle(f, v, size_debug);
	//

	int y, x;

	y = 0;
	coord.y = (int)g_man.player.pos.y - 4;
	while (coord.y < (int)g_man.player.pos.y + 5)
	{
		x = 0;
		coord.x = (int)g_man.player.pos.x - 4;
		while (coord.x < (int)g_man.player.pos.x + 5)
		{
			v.coord.x = offset.x + (x + ((int)g_man.player.pos.x - g_man.player.pos.x)) * cell_size.x;
			v.coord.y = offset.y + (y + ((int)g_man.player.pos.y - g_man.player.pos.y)) * cell_size.y;

			//if (v.coord.x >= offset.x && v.coord.y >= offset.y
			//	&& v.coord.x < offset.x + 8 * cell_size.x && v.coord.y < offset.y + 8 * cell_size.y)
			/*
				What size for first cell?
			*/
			if (x == 0)
			{
				//printf("pos.x = %f / pos.x - (int)pos.x = %f\n", g_man.player.pos.x, 
				//	g_man.player.pos.x - (int)g_man.player.pos.x);
				//cell_size.x = 9 * (g_man.player.pos.x - (int)g_man.player.pos.x);
			}
			else
				cell_size.x = 9;

			if (coord.x >= 0 && coord.y >= 0
				&& coord.x < m->size.x && coord.y < m->size.y)
			{
				v.color = get_color_hex("264653", 255); //empty
				if (m->cells[coord.y * m->size.x + coord.x].is_goal)
					v.color = get_color_hex("1CD2C0", 255); //obelisk
				else if (m->cells[coord.y * m->size.x + coord.x].is_door)
					v.color = get_color_hex("f4a261", 255); //door
				else if (m->cells[coord.y * m->size.x + coord.x].is_obstacle)
					v.color = get_color_hex("287271", 255); //wall
				// e76f51 enemy
				cell_size.x = 9 - 1;
				cell_size.y = 9 - 1;
				draw_rectangle_full(f, v, cell_size);
			}
			cell_size.x = 9;
			cell_size.y = 9;
			++x;
			++coord.x;
		}
		++y;
		++coord.y;
	}

	// DRAW PLAYER IN THE CENTER
	v.coord.x = offset.x + 3.75 * cell_size.x;
	v.coord.y = offset.y + 3.75 * cell_size.y;
	v.color = get_color_hex("e9c46a", 255); //player
	cell_size.x *= 0.5;
	cell_size.y *= 0.5;
	draw_rectangle_full(f, v, cell_size);
	return ;
}
