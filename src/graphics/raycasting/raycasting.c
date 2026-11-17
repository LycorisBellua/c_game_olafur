#include "olafur.h"

static void	reset_column_buffers(t_man *man);
static void	process_dda_list(t_man *man, int x);
static void	set_line(t_man *man, int x, t_ray *r);

void	raycasting(t_man *man)
{
	int	x;

	reset_column_buffers(man);
	cast_floor_and_ceiling(man);
	sort_sprites_by_dist(man);
	x = 0;
	while (x < man->frame.size.x)
	{
		perform_dda(man, 2 * x / (float)man->frame.size.x - 1);
		process_dda_list(man, x);
		++x;
	}
	return ;
}

/*
	Distances along a ray are measured in multiples of its direction vector, 
	which is longer towards the edges of the screen. Multiplying by its length 
	gives the actual distance, which the fog uses so that it doesn't change 
	when rotating.
*/
static void	reset_column_buffers(t_man *man)
{
	t_vec2	ray_dir;
	float	cam_x;
	int		i;

	i = 0;
	while (i < man->frame.size.x)
	{
		cam_x = 2 * i / (float)man->frame.size.x - 1;
		ray_dir.x = man->player.dir.x + man->player.plane.x * cam_x;
		ray_dir.y = man->player.dir.y + man->player.plane.y * cam_x;
		man->ray_len[i] = sqrtf(ray_dir.x * ray_dir.x + ray_dir.y * ray_dir.y);
		man->z_buf[i++] = HUGE_VALF;
	}
	return ;
}

static void	process_dda_list(t_man *man, int x)
{
	t_ray	*r;
	int		i;

	i = man->ray_count;
	while (i > 0)
	{
		--i;
		r = &man->ray_buf[i];
		if (!(r->m_index.x < 0 || r->m_index.x >= r->m->size.x
			|| r->m_index.y < 0 || r->m_index.y >= r->m->size.y))
		{
			if (r->is_see_through)
				cast_sprites(man, x, r->perp_wall_dist);
			if (r->perp_wall_dist < man->z_buf[x])
				man->z_buf[x] = r->perp_wall_dist;
			set_line(man, x, r);
			draw_wall(man, r, r->tex);
			draw_wall(man, r, r->tex_portal);
		}
	}
	cast_sprites(man, x, 0.0f);
	return ;
}

/*
	The drawn height is capped so that it, and the positions worked out from 
	it, always fit in an int, even right up against a tall wall.
*/
static void	set_line(t_man *man, int x, t_ray *r)
{
	t_cell	*cell;
	float	offset;
	float	pwd;

	cell = &r->m->cells[r->m_index.y][r->m_index.x];
	pwd = r->perp_wall_dist;
	if (pwd < EPSILON)
		pwd = EPSILON;
	r->line_height_cubic = fclamp(man->frame.size.y / pwd, 1.0f,
			MAX_LINE_HEIGHT / cell->height);
	r->line_height = r->line_height_cubic * cell->height;
	offset = r->line_height_cubic * (1.0f - cell->height) * 0.5f;
	r->unclamped_line_height.x = -r->line_height / 2 + man->frame.size.y / 2
		+ offset;
	r->unclamped_line_height.y = r->line_height / 2 + man->frame.size.y / 2
		+ offset;
	r->coord1.x = x;
	r->coord1.y = imax(r->unclamped_line_height.x, 0);
	r->coord2.x = x;
	r->coord2.y = imin(r->unclamped_line_height.y, man->frame.size.y - 1);
	r->fog_factor8 = (int)(fog_factor_of(r->perp_wall_dist * r->ray_len,
				man->dof) * 256.0f);
	return ;
}
