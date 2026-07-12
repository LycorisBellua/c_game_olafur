#include "olafur.h"

static void	set_sprite_values(t_man *man, t_spr *s);
static void	render_sprite_column(t_man *man, t_spr *s, int x);
static void	blit_sprite_pixel(t_man *man, t_spr *s, t_color c,
				t_ivec2 screen, t_color fog);

void	sort_sprites_by_dist(t_man *man)
{
	t_spr	**spr;
	t_spr	*key;
	int		i;
	int		j;

	spr = man->maps[man->curr_map]->sprites;
	i = 0;
	while (i < man->maps[man->curr_map]->sprite_len)
	{
		spr[i]->dist = dist(man->player.pos, spr[i]->pos);
		++i;
	}
	i = 1;
	while (i < man->maps[man->curr_map]->sprite_len)
	{
		key = spr[i];
		j = i - 1;
		while (j >= 0 && spr[j]->dist < key->dist)
		{
			spr[j + 1] = spr[j];
			--j;
		}
		spr[j + 1] = key;
		++i;
	}
	return ;
}

void	cast_sprites(t_man *man, int x, float near_bound)
{
	int		i;
	t_spr	*s;

	i = 0;
	while (i < man->maps[man->curr_map]->sprite_len)
	{
		s = man->maps[man->curr_map]->sprites[i];
		if (!x)
			set_sprite_values(man, s);
		if (s->img && s->transform.y <= man->dof
			&& x >= s->draw_start.x && x < s->draw_end.x
			&& s->transform.y > near_bound
			&& s->transform.y < man->z_buf[x])
			render_sprite_column(man, s, x);
		++i;
	}
	return ;
}

static void	set_sprite_values(t_man *man, t_spr *s)
{
	const int		U_DIV = 1;
	const int		V_DIV = 1;
	const float	V_MOVE = 0.0;
	t_vec2			pos;
	float			inv_det;

	set_vec2(&pos, s->pos.x - man->player.pos.x, s->pos.y - man->player.pos.y);
	inv_det = 1.0 / ((man->player.plane.x * man->player.dir.y
				- man->player.dir.x * man->player.plane.y));
	s->transform.x = (man->player.dir.y * pos.x - man->player.dir.x * pos.y)
		* inv_det;
	s->transform.y = (-man->player.plane.y * pos.x + man->player.plane.x
			* pos.y) * inv_det;
	s->fog_factor8 = (int)(fog_factor_of(s->transform.y, man->dof) * 256.0f);
	if (s->transform.y <= EPSILON)
	{
		s->draw_start.x = 0;
		s->draw_end.x = 0;
		return ;
	}
	s->screen_x = (man->frame.size.x / 2)
		* (1 + s->transform.x / s->transform.y);
	s->v_move_screen = V_MOVE / s->transform.y;
	s->size.y = abs((int)(man->frame.size.y / s->transform.y)) / V_DIV;
	s->draw_start.y = imax(-s->size.y / 2 + man->frame.size.y / 2
			+ s->v_move_screen, 0);
	s->draw_end.y = imin(s->size.y / 2 + man->frame.size.y / 2
			+ s->v_move_screen, man->frame.size.y - 1);
	s->size.x = abs((int)(man->frame.size.y / s->transform.y)) / U_DIV;
	s->draw_start.x = imax(-s->size.x / 2 + s->screen_x, 0);
	s->draw_end.x = imin(s->size.x / 2 + s->screen_x, man->frame.size.x);
	return ;
}

static void	blit_sprite_pixel(t_man *man, t_spr *s, t_color c, t_ivec2 screen,
				t_color fog)
{
	if (c.a == 0)
		return ;
	apply_fog_factor(&c, fog, s->fog_factor8);
	draw_point_fast(man, c, screen.x, screen.y);
	return ;
}

static void	render_sprite_column(t_man *man, t_spr *s, int x)
{
	t_ivec2	tex;
	t_color	*frame;
	long	rem;
	long	den;
	int		y;
	int		col_base;
	t_color	fog;

	tex.x = (int)(256 * (x - (-s->size.x / 2 + s->screen_x)) * s->img->size.x
			/ s->size.x) / 256;
	if (tex.x < 0 || tex.x >= s->img->size.x)
		return ;
	frame = s->img->cycle[s->img->cycle_index];
	col_base = tex.x * s->img->size.y;
	fog = man->maps[man->curr_map]->fog_color;
	den = (long)s->size.y * 256;
	y = s->draw_start.y;
	rem = ((long)(y - s->v_move_screen) * 256 - (long)man->frame.size.y * 128
			+ (long)s->size.y * 128) * s->img->size.y;
	tex.y = rem / den;
	rem -= tex.y * den;
	while (y < s->draw_end.y)
	{
		if (tex.y < 0 || tex.y >= s->img->size.y)
			return ;
		blit_sprite_pixel(man, s, frame[col_base + tex.y],
			(t_ivec2){x, y}, fog);
		rem += (long)s->img->size.y * 256;
		while (rem >= den)
		{
			++tex.y;
			rem -= den;
		}
		++y;
	}
	return ;
}
