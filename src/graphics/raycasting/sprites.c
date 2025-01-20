#include "cimmerian.h"

typedef struct s_sprite
{
	double	x;
	double	y;
	int		texture;
}	t_sprite;

t_sprite sprites[NUM_SPRITES] =
{
	//green lights
	{20.5, 11.5, 12},
	{18.5,  4.5, 12},
	{10.0,  4.5, 12},
	{10.0, 12.5, 12},
	{ 3.5,  6.5, 12},
	{ 3.5, 20.5, 12},
	{ 3.5, 14.5, 12},
	{14.5, 20.5, 12},

	//pillars
	{18.5, 10.5, 11},
	{18.5, 11.5, 11},
	{18.5, 12.5, 11},
	{ 8.5,  7.0, 11},

	//barrels
	{21.5,  1.5, 10},
	{15.5,  1.5, 10},
	{16.0,  1.8, 10},
	{16.2,  1.2, 10},
	{ 3.5,  2.5, 10},
	{ 9.5, 15.5, 10},
	{10.0, 15.1, 10},
	{10.5, 15.8, 10},
};

static void render_sprite_column(t_frame *f, t_map *m, t_sprite *s,
	double grid_dist, double *z_buffer, int x);

void	sort_sprites(int *sprite_order, double *sprite_dist, int sprite_amount)
{
	int		i;
	int		j;
	double	tmp_dist;
	double	tmp_order;

	i = 0;
	while (i < NUM_SPRITES)
	{
		sprite_order[i] = i;
		sprite_dist[i] = get_squared_dist_euclidean(g_man.player.pos.x,
			g_man.player.pos.y, sprites[i].x, sprites[i].y);
		++i;
	}
	i = 0;
	while (i < sprite_amount - 1)
	{
		j = 0;
		while (j < sprite_amount - i - 1)
		{
			if (sprite_dist[j] < sprite_dist[j + 1])
			{
				tmp_dist = sprite_dist[j];
				sprite_dist[j] = sprite_dist[j + 1];
				sprite_dist[j + 1] = tmp_dist;
				tmp_order = sprite_order[j];
				sprite_order[j] = sprite_order[j + 1];
				sprite_order[j + 1] = tmp_order;
			}
			++j;
		}
		++i;
	}
	return ;
}

void cast_sprites(t_frame *f, t_map *m, double *z_buffer, int *sprite_order,
	double *sprite_dist, int x)
{
	int	i;

	i = 0;
	// Loop through every sprite
	while (i < NUM_SPRITES)
	{
		double grid_dist = f_sqrt(sprite_dist[i]);

		// Only render the sprite if it's within the distance of focus
		if (grid_dist <= m->dof)
			render_sprite_column(f, m, sprites + sprite_order[i], grid_dist, z_buffer, x);
		++i;
	}
	return ;
}

// Render only a specific column of the sprite
static void render_sprite_column(t_frame *f, t_map *m, t_sprite *s,
	double grid_dist, double *z_buffer, int x)
{
	t_vec2 sprite;
	double inv_det;
	t_vec2 transform;
	int sprite_screen_x;
	int v_move_screen;
	int sprite_width;
	int sprite_height;
	t_ivec2 draw_start;
	t_ivec2 draw_end;
	int tex_width;
	int tex_height;
	t_ivec2 tex;
	int y;
	int d;

	// Translate sprite position to relative to camera
	sprite.x = s->x - g_man.player.pos.x;
	sprite.y = s->y - g_man.player.pos.y;

	// Transform sprite with the inverse camera matrix
	inv_det = 1.0 / ((g_man.player.plane.x * g_man.player.dir.y
		- g_man.player.dir.x * g_man.player.plane.y) * g_man.res.h_mod);

	transform.x = inv_det * (g_man.player.dir.y * sprite.x - g_man.player.dir.x * sprite.y);
	transform.y = inv_det * (-g_man.player.plane.y * sprite.x + g_man.player.plane.x * sprite.y);

	sprite_screen_x = (int)((f->size.x / 2) * (1 + transform.x / transform.y));

	const int u_div = 1;
	const int v_div = 1;
	const double v_move = 0.0;
	v_move_screen = (int)(v_move / transform.y);

	sprite_height = abs((int)(f->size.y / transform.y)) / v_div;
	draw_start.y = max(-sprite_height / 2 + f->size.y / 2 + v_move_screen, 0);
	draw_end.y = min(sprite_height / 2 + f->size.y / 2 + v_move_screen, f->size.y - 1);

	sprite_width = abs((int)(f->size.y / transform.y)) / u_div;
	draw_start.x = max(-sprite_width / 2 + sprite_screen_x, 0);
	draw_end.x = min(sprite_width / 2 + sprite_screen_x, f->size.x);

	if (x >= draw_start.x && x < draw_end.x)
	{
		tex_width = m->img[s->texture]->size.x;
		tex_height = m->img[s->texture]->size.y;
		tex.x = (int)(256 * (x - (-sprite_width / 2 + sprite_screen_x))
			* tex_width / sprite_width) / 256;

		if (transform.y > 0 && transform.y < z_buffer[x] / g_man.res.h_mod)
		{
			y = draw_start.y;
			while (y < draw_end.y)
			{
				d = (y - v_move_screen) * 256 - f->size.y * 128 + sprite_height * 128;
				tex.y = ((d * tex_height) / sprite_height) / 256;
				t_color color = m->img[s->texture]->buf[tex_width * tex.y + tex.x];
				apply_wall_fog(&color, m->fog_color, grid_dist, m->dof);
				draw_point(f, color, x, y);
				++y;
			}
		}
	}
	return ;
}
