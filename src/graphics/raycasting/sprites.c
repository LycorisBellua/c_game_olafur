#include "cimmerian.h"

static void	set_sprite_values(t_man *man, t_spr *s);
static void	render_sprite_column(t_man *man, t_spr *s, int x);

void	sort_sprites_by_dist(t_man *man)
{
	int	i;
	int	j;

	i = 0;
	while (i < man->maps[man->curr_map]->sprite_len)
	{
		man->maps[man->curr_map]->sprites[i]->dist = dist(man->player.pos,
				man->maps[man->curr_map]->sprites[i]->pos);
		++i;
	}
	i = 0;
	while (i < man->maps[man->curr_map]->sprite_len - 1)
	{
		j = 0;
		while (j < man->maps[man->curr_map]->sprite_len - i - 1)
		{
			if (man->maps[man->curr_map]->sprites[j]->dist
				< man->maps[man->curr_map]->sprites[j + 1]->dist)
				swap_elements((void **)(man->maps[man->curr_map]->sprites + j),
					(void **)(man->maps[man->curr_map]->sprites + j + 1));
			++j;
		}
		++i;
	}
	return ;
}

void	cast_sprites(t_man *man, int x)
{
	int		i;
	t_spr	*s;

	i = 0;
	while (i < man->maps[man->curr_map]->sprite_len)
	{
		s = man->maps[man->curr_map]->sprites[i];
		if (s->dist <= man->dof)
		{
			if (!x)
				set_sprite_values(man, s);
			if (s->img && x >= s->draw_start.x && x < s->draw_end.x
				&& s->transform.y > 0 && s->transform.y < man->z_buf[x])
				render_sprite_column(man, s, x);
		}
		++i;
	}
	return ;
}

static void	set_sprite_values(t_man *man, t_spr *s)
{
	const int		U_DIV = 1;
	const int		V_DIV = 1;
	const double	V_MOVE = 0.0;
	t_vec2			pos;
	double			inv_det;

	set_vec2(&pos, s->pos.x - man->player.pos.x, s->pos.y - man->player.pos.y);
	inv_det = 1.0 / ((man->player.plane.x * man->player.dir.y
				- man->player.dir.x * man->player.plane.y));
	s->transform.x = (man->player.dir.y * pos.x - man->player.dir.x * pos.y)
		* inv_det;
	s->transform.y = (-man->player.plane.y * pos.x + man->player.plane.x
			* pos.y) * inv_det;
	s->screen_x = (man->frame.size.x / 2)
		* (1 + s->transform.x / s->transform.y);
	s->v_move_screen = V_MOVE / s->transform.y;
	s->size.y = abs((int)(man->frame.size.y / s->transform.y)) / V_DIV;
	s->draw_start.y = max(-s->size.y / 2 + man->frame.size.y / 2
			+ s->v_move_screen, 0);
	s->draw_end.y = min(s->size.y / 2 + man->frame.size.y / 2
			+ s->v_move_screen, man->frame.size.y - 1);
	s->size.x = abs((int)(man->frame.size.y / s->transform.y)) / U_DIV;
	s->draw_start.x = max(-s->size.x / 2 + s->screen_x, 0);
	s->draw_end.x = min(s->size.x / 2 + s->screen_x, man->frame.size.x);
	return ;
}

static void	render_sprite_column(t_man *man, t_spr *s, int x)
{
	t_ivec2	tex;
	int		y;
	int		d;
	t_color	c;

	tex.x = (int)(256 * (x - (-s->size.x / 2 + s->screen_x)) * s->img->size.x
		/ s->size.x) / 256;
	if (tex.x < 0 || tex.x >= man->frame.size.x)
		return ;
	y = s->draw_start.y - 1;
	while (++y < s->draw_end.y)
	{
		d = (y - s->v_move_screen) * 256 - man->frame.size.y * 128 + s->size.y
			* 128;
		tex.y = ((d * s->img->size.y) / s->size.y) / 256;
		if (tex.y < 0 || tex.y >= man->frame.size.y)
			return ;
		c = s->img->cycle[s->img->cycle_index][tex.y * s->img->size.x + tex.x];
		apply_wall_fog(&c, man->maps[man->curr_map]->fog_color, s->dist,
			man->dof);
		draw_point(man, c, x, y);
	}
	return ;
}
