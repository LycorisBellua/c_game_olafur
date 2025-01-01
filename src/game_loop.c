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

	set_ivec2(&cell_size, 9, 9);
	set_ivec2(&padding, 5, 5);
	set_ivec2(&offset, f->size.x - m->size.x * cell_size.x - padding.x,
		padding.y);
	coord.y = 0;
	while (coord.y < m->size.y)
	{
		coord.x = 0;
		while (coord.x < m->size.x)
		{
			t_vert v;
			v.coord.x = offset.x + coord.x * cell_size.x;
			v.coord.y = offset.y + coord.y * cell_size.y;
			v.color = get_color_hex("264653", 255); //empty
			if (m->cells[coord.y * m->size.x + coord.x].is_goal)
				v.color = get_color_hex("1CD2C0", 255); //obelisk
			else if (m->cells[coord.y * m->size.x + coord.x].is_door)
				v.color = get_color_hex("f4a261", 255); //door
			else if (m->cells[coord.y * m->size.x + coord.x].is_obstacle)
				v.color = get_color_hex("287271", 255); //wall
			// e76f51 enemy
			--cell_size.x;
			--cell_size.y;
			draw_rectangle_full(f, v, cell_size);
			++cell_size.x;
			++cell_size.y;
			++coord.x;
		}
		++coord.y;
	}

	t_vec2	player_size_factor;
	set_vec2(&player_size_factor, 0.5, 0.5);
	coord.x = g_man.player.pos.x;
	coord.y = g_man.player.pos.y;
	t_vert v;
	v.coord.x = offset.x + coord.x * cell_size.x
		+ (g_man.player.pos.x - (0.5 * player_size_factor.x)) * cell_size.x
		- (int)g_man.player.pos.x * cell_size.x;
	v.coord.y = offset.y + coord.y * cell_size.y
		+ (g_man.player.pos.y - (0.5 * player_size_factor.y)) * cell_size.y
		- (int)g_man.player.pos.y * cell_size.y;
	v.color = get_color_hex("e9c46a", 255); //player
	cell_size.x *= player_size_factor.x;
	cell_size.y *= player_size_factor.y;
	cell_size.x = f_min(4, cell_size.x);
	cell_size.y = f_min(4, cell_size.y);
	draw_rectangle_full(f, v, cell_size);

	return ;
}
