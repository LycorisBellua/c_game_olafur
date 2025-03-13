#include "cimmerian.h"

static void render_sprite_column(t_man *man, t_frame *f, t_spr *s,
				double grid_dist, double *z_buffer, int x);

void	sort_sprites(t_man *man, int *spr_order, double *spr_dist, int spr_amount)
{
	int		i;
	int		j;
	double	tmp_dist;
	double	tmp_order;

	i = 0;
	while (i < NBR_SPR)
	{
		spr_order[i] = i;
		spr_dist[i] = get_squared_dist_euclidean(man->player.pos.x,
			man->player.pos.y, man->map->sprites[i].pos.x, man->map->sprites[i].pos.y);
		++i;
	}
	i = 0;
	while (i < spr_amount - 1)
	{
		j = 0;
		while (j < spr_amount - i - 1)
		{
			if (spr_dist[j] < spr_dist[j + 1])
			{
				tmp_dist = spr_dist[j];
				spr_dist[j] = spr_dist[j + 1];
				spr_dist[j + 1] = tmp_dist;
				tmp_order = spr_order[j];
				spr_order[j] = spr_order[j + 1];
				spr_order[j + 1] = tmp_order;
			}
			++j;
		}
		++i;
	}
	return ;
}

void cast_sprites(t_man *man, t_frame *f, double *z_buffer, int *spr_order,
	double *spr_dist, int x)
{
	int	i;

	i = 0;
	// Loop through every sprite
	while (i < NBR_SPR)
	{
		double grid_dist = sqrt_f(spr_dist[i]);

		// Only render the sprite if it's within the distance of focus
		if (grid_dist <= man->map->dof)
			render_sprite_column(man, f, man->map->sprites + spr_order[i], grid_dist, z_buffer, x);
		++i;
	}
	return ;
}

// Render only a specific column of the sprite
static void render_sprite_column(t_man *man, t_frame *f, t_spr *s,
	double grid_dist, double *z_buffer, int x)
{
	t_vec2 sprite;
	double inv_det;
	t_vec2 transform;
	int spr_screen_x;
	int v_move_screen;
	int spr_width;
	int spr_height;
	t_ivec2 draw_start;
	t_ivec2 draw_end;
	int tex_width;
	int tex_height;
	t_ivec2 tex;
	int y;
	int d;

	// Translate sprite position to relative to camera
	sprite.x = s->pos.x - man->player.pos.x;
	sprite.y = s->pos.y - man->player.pos.y;

	// Transform sprite with the inverse camera matrix
	inv_det = 1.0 / ((man->player.plane.x * man->player.dir.y
		- man->player.dir.x * man->player.plane.y) * man->res.h_mod);

	transform.x = inv_det * (man->player.dir.y * sprite.x - man->player.dir.x * sprite.y);
	transform.y = inv_det * (-man->player.plane.y * sprite.x + man->player.plane.x * sprite.y);

	spr_screen_x = (int)((f->size.x / 2) * (1 + transform.x / transform.y));

	const int u_div = 1;
	const int v_div = 1;
	const double v_move = 0.0;
	v_move_screen = (int)(v_move / transform.y);

	spr_height = abs((int)(f->size.y / transform.y)) / v_div;
	draw_start.y = max(-spr_height / 2 + f->size.y / 2 + v_move_screen, 0);
	draw_end.y = min(spr_height / 2 + f->size.y / 2 + v_move_screen, f->size.y - 1);

	spr_width = abs((int)(f->size.y / transform.y)) / u_div;
	draw_start.x = max(-spr_width / 2 + spr_screen_x, 0);
	draw_end.x = min(spr_width / 2 + spr_screen_x, f->size.x);

	if (x >= draw_start.x && x < draw_end.x)
	{
		tex_width = s->img->size.x;
		tex_height = s->img->size.y;
		tex.x = (int)(256 * (x - (-spr_width / 2 + spr_screen_x))
			* tex_width / spr_width) / 256;

		if (transform.y > 0 && transform.y < z_buffer[x] / man->res.h_mod)
		{
			y = draw_start.y;
			while (y < draw_end.y)
			{
				d = (y - v_move_screen) * 256 - f->size.y * 128 + spr_height * 128;
				tex.y = ((d * tex_height) / spr_height) / 256;
				t_color color = s->img->cycle[s->img->cycle_index][tex_width * tex.y + tex.x];
				apply_wall_fog(&color, man->map->fog_color, grid_dist, man->map->dof);
				draw_point(f, color, x, y);
				++y;
			}
		}
	}
	return ;
}
