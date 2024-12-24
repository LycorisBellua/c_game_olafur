#include "cimmerian.h"

static void		wall_flat_color(t_map *m, t_frame *f, t_ray *r);
static void		wall_texturing(t_map *m, t_frame *f, t_ray *r);
static t_img	*get_texture(t_map *m, t_ray *r);

void	draw_wall(t_map *m, t_frame *f, t_ray *r)
{
	if (r->perp_wall_dist > m->dof * g_man.tex_in_dof)
		wall_flat_color(m, f, r);
	else
		wall_texturing(m, f, r);
	return ;
}

static void	wall_flat_color(t_map *m, t_frame *f, t_ray *r)
{
	t_vert	v1;
	t_vert	v2;
	t_color	color;

	v1.coord = r->coord1;
	v2.coord = r->coord2;
	color = m->cells[r->m_index.y * m->size.x + r->m_index.x]
		.tex_north->average_color;
	if (r->side == 1)
	{
		color.r /= 2;
		color.g /= 2;
		color.b /= 2;
	}
	apply_wall_fog(&color, m->fog_color, r->perp_wall_dist, m->dof);
	v1.color = color;
	v2.color = color;
	draw_line(f, v1, v2);
	return ;
}

static void	wall_texturing(t_map *m, t_frame *f, t_ray *r)
{
	t_img	*img;

	img = get_texture(m, r);
	if (!img)
		return ;

    // Where exactly the wall was hit
	double	wall_x;
	if (r->side == 0)
		wall_x = g_man.player.pos.y + r->perp_wall_dist * r->ray_dir.y;
	else
		wall_x = g_man.player.pos.x + r->perp_wall_dist * r->ray_dir.x;
	wall_x -= f_floor(wall_x);

	// X coordinate on the image
	t_ivec2	img_coord;
	img_coord.x = (int)(wall_x * (double)img->size.x);
	if ((r->side == 0 && r->ray_dir.x < 0)
		|| (r->side == 1 && r->ray_dir.y > 0))
		img_coord.x = img->size.x - img_coord.x - 1;

	// How much to increase the image coordinate per screen pixel
	double	img_step;
	img_step = 1.0 * img->size.y / r->line_height;

	// Starting image coordinate
	double img_pos;
	img_pos = (r->coord1.y - (f->size.y / 2.0 - r->line_height / 2.0))
		* img_step;

	t_color	*img_buf;
	t_color	color;
	int		y;
	img_buf = (t_color *)img->buf;
	y = r->coord1.y;
	while (y < r->coord2.y)
	{
		// Cast the image coordinate to integer, and clamp to [0, IMG_H - 1]
		img_coord.y = (int)img_pos % img->size.y;
		img_pos += img_step;

		color = img_buf[img_coord.y * img->size.x + img_coord.x];

		// Make color darker for y-sides
		if (r->side == 1)
		{
			color.r /= 2;
			color.g /= 2;
			color.b /= 2;
		}
		apply_wall_fog(&color, m->fog_color, r->perp_wall_dist, m->dof);
		draw_point(f, color, r->coord1.x, y);
		++y;
	}
	return ;
}

static t_img	*get_texture(t_map *m, t_ray *r)
{
	if (r->side == 1 && r->ray_dir.y > 0)
		return (m->cells[r->m_index.y * m->size.x + r->m_index.x].tex_north);
	else if (r->side == 1 && r->ray_dir.y < 0)
		return (m->cells[r->m_index.y * m->size.x + r->m_index.x].tex_south);
	else if (r->side == 0 && r->ray_dir.x > 0)
		return (m->cells[r->m_index.y * m->size.x + r->m_index.x].tex_west);
	else if (r->side == 0 && r->ray_dir.x < 0)
		return (m->cells[r->m_index.y * m->size.x + r->m_index.x].tex_east);
	return (0);
}
