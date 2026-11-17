#include "olafur.h"

static void	set_sprite_values(t_man *man, t_spr *s);
static void	set_sprite_transform(t_man *man, t_spr *s);

void	sort_sprites_by_dist(t_man *man)
{
	t_spr	**spr;
	t_spr	*key;
	int		i;
	int		j;

	spr = man->maps[man->curr_map]->sprites;
	i = -1;
	while (++i < man->maps[man->curr_map]->sprite_len)
		spr[i]->dist = dist(man->player.pos, spr[i]->pos);
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
		if (s->img && s->dist <= man->dof
			&& x >= s->draw_start.x && x < s->draw_end.x
			&& s->transform.y > near_bound
			&& s->transform.y < man->z_buf[x])
			render_sprite_column(man, s, x);
		++i;
	}
	return ;
}

/*
	A sprite almost level with the camera isn't drawn: it would be far larger 
	than the screen, and its size and position wouldn't fit in an int.
*/
static void	set_sprite_values(t_man *man, t_spr *s)
{
	set_sprite_transform(man, s);
	s->fog_factor8 = (int)(fog_factor_of(s->dist, man->dof) * 256.0f);
	if (s->transform.y <= SPRITE_NEAR)
	{
		s->draw_start.x = 0;
		s->draw_end.x = 0;
		return ;
	}
	s->screen_x = (man->frame.size.x / 2)
		* (1 + s->transform.x / s->transform.y);
	s->v_move_screen = SPRITE_V_MOVE / s->transform.y;
	s->size.y = abs((int)(man->frame.size.y / s->transform.y)) / SPRITE_V_DIV;
	s->draw_start.y = imax(-s->size.y / 2 + man->frame.size.y / 2
		+ s->v_move_screen, 0);
	s->draw_end.y = imin(s->size.y / 2 + man->frame.size.y / 2
		+ s->v_move_screen, man->frame.size.y - 1);
	s->size.x = abs((int)(man->frame.size.y / s->transform.y)) / SPRITE_U_DIV;
	s->draw_start.x = imax(-s->size.x / 2 + s->screen_x, 0);
	s->draw_end.x = imin(s->size.x / 2 + s->screen_x, man->frame.size.x);
	return ;
}

static void	set_sprite_transform(t_man *man, t_spr *s)
{
	t_vec2	pos;
	float	inv_det;

	set_vec2(&pos, s->pos.x - man->player.pos.x, s->pos.y - man->player.pos.y);
	inv_det = 1.0f / ((man->player.plane.x * man->player.dir.y
		- man->player.dir.x * man->player.plane.y));
	s->transform.x = (man->player.dir.y * pos.x - man->player.dir.x * pos.y)
		* inv_det;
	s->transform.y = (-man->player.plane.y * pos.x + man->player.plane.x
		* pos.y) * inv_det;
	return ;
}
