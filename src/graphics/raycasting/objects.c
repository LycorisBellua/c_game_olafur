#include "cimmerian.h"

static void render_object_column(t_frame *f, t_map *m, t_obj *s,
	double grid_dist, double *z_buffer, int x);

void	sort_objects(int *object_order, double *object_dist, int object_amount)
{
	int		i;
	int		j;
	double	tmp_dist;
	double	tmp_order;

	i = 0;
	while (i < NBR_OBJ)
	{
		object_order[i] = i;
		object_dist[i] = get_squared_dist_euclidean(g_man.player.pos.x,
			g_man.player.pos.y, g_man.map->objects[i].pos.x, g_man.map->objects[i].pos.y);
		++i;
	}
	i = 0;
	while (i < object_amount - 1)
	{
		j = 0;
		while (j < object_amount - i - 1)
		{
			if (object_dist[j] < object_dist[j + 1])
			{
				tmp_dist = object_dist[j];
				object_dist[j] = object_dist[j + 1];
				object_dist[j + 1] = tmp_dist;
				tmp_order = object_order[j];
				object_order[j] = object_order[j + 1];
				object_order[j + 1] = tmp_order;
			}
			++j;
		}
		++i;
	}
	return ;
}

void cast_objects(t_frame *f, t_map *m, double *z_buffer, int *object_order,
	double *object_dist, int x)
{
	int	i;

	i = 0;
	// Loop through every object
	while (i < NBR_OBJ)
	{
		double grid_dist = f_sqrt(object_dist[i]);

		// Only render the object if it's within the distance of focus
		if (grid_dist <= m->dof)
			render_object_column(f, m, g_man.map->objects + object_order[i], grid_dist, z_buffer, x);
		++i;
	}
	return ;
}

// Render only a specific column of the object
static void render_object_column(t_frame *f, t_map *m, t_obj *s,
	double grid_dist, double *z_buffer, int x)
{
	t_vec2 object;
	double inv_det;
	t_vec2 transform;
	int object_screen_x;
	int v_move_screen;
	int object_width;
	int object_height;
	t_ivec2 draw_start;
	t_ivec2 draw_end;
	int tex_width;
	int tex_height;
	t_ivec2 tex;
	int y;
	int d;

	// Translate object position to relative to camera
	object.x = s->pos.x - g_man.player.pos.x;
	object.y = s->pos.y - g_man.player.pos.y;

	// Transform object with the inverse camera matrix
	inv_det = 1.0 / ((g_man.player.plane.x * g_man.player.dir.y
		- g_man.player.dir.x * g_man.player.plane.y) * g_man.res.h_mod);

	transform.x = inv_det * (g_man.player.dir.y * object.x - g_man.player.dir.x * object.y);
	transform.y = inv_det * (-g_man.player.plane.y * object.x + g_man.player.plane.x * object.y);

	object_screen_x = (int)((f->size.x / 2) * (1 + transform.x / transform.y));

	const int u_div = 1;
	const int v_div = 1;
	const double v_move = 0.0;
	v_move_screen = (int)(v_move / transform.y);

	object_height = abs((int)(f->size.y / transform.y)) / v_div;
	draw_start.y = max(-object_height / 2 + f->size.y / 2 + v_move_screen, 0);
	draw_end.y = min(object_height / 2 + f->size.y / 2 + v_move_screen, f->size.y - 1);

	object_width = abs((int)(f->size.y / transform.y)) / u_div;
	draw_start.x = max(-object_width / 2 + object_screen_x, 0);
	draw_end.x = min(object_width / 2 + object_screen_x, f->size.x);

	if (x >= draw_start.x && x < draw_end.x)
	{
		tex_width = s->spr->size.x;
		tex_height = s->spr->size.y;
		tex.x = (int)(256 * (x - (-object_width / 2 + object_screen_x))
			* tex_width / object_width) / 256;

		if (transform.y > 0 && transform.y < z_buffer[x] / g_man.res.h_mod)
		{
			y = draw_start.y;
			while (y < draw_end.y)
			{
				d = (y - v_move_screen) * 256 - f->size.y * 128 + object_height * 128;
				tex.y = ((d * tex_height) / object_height) / 256;
				t_color color = s->spr->cycle[s->spr->cycle_index][tex_width * tex.y + tex.x];
				apply_wall_fog(&color, m->fog_color, grid_dist, m->dof);
				draw_point(f, color, x, y);
				++y;
			}
		}
	}
	return ;
}
