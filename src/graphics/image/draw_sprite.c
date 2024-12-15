#include "cimmerian.h"

static void	draw_shadow(t_frame *frame, t_spr *sprite, t_ivec2 pos);
static void	draw_regular(t_frame *frame, t_spr *sprite, t_ivec2 pos);

void	draw_sprite(t_frame *frame, t_spr *sprite, t_ivec2 pos, long dt_ms)
{
	if (!sprite)
		return ;
	if (!dt_ms)
		sprite->cycle_index = sprite->still_frame;
	if (sprite->cycle_shadow)
		draw_shadow(frame, sprite, pos);
	draw_regular(frame, sprite, pos);
	if (!dt_ms || !sprite->cycle_time_in_ms)
		return ;
	sprite->elapsed_time_in_ms += dt_ms;
	while (sprite->elapsed_time_in_ms >= sprite->cycle_time_in_ms)
	{
		sprite->cycle_index = (sprite->cycle_index + 1) % sprite->cycle_len;
		sprite->elapsed_time_in_ms -= sprite->cycle_time_in_ms;
	}
	return ;
}

t_spr	*get_sprite(char *id)
{
	int	i;

	i = 0;
	while (g_man.map->sprites[i].id)
	{
		if (!strcmp(g_man.map->sprites[i].id, id))
			return (&g_man.map->sprites[i]);
		++i;
	}
	return (0);
}

static void	draw_shadow(t_frame *frame, t_spr *sprite, t_ivec2 pos)
{
	size_t	i;
	size_t	len;
	t_ivec2	p;

	pos.x += sprite->shadow_offset.x;
	pos.y += sprite->shadow_offset.y;
	i = 0;
	len = sprite->size.x * sprite->size.y;
	while (i < len)
	{
		p.y = i / sprite->size.x;
		p.x = i - p.y * sprite->size.x;
		set_ivec2(&p, p.x + pos.x, p.y + pos.y);
		draw_point(frame, sprite->cycle_shadow[sprite->cycle_index][i], p.x,
			p.y);
		++i;
	}
	return ;
}

static void	draw_regular(t_frame *frame, t_spr *sprite, t_ivec2 pos)
{
	size_t	i;
	size_t	len;
	t_ivec2	p;

	i = 0;
	len = sprite->size.x * sprite->size.y;
	while (i < len)
	{
		p.y = i / sprite->size.x;
		p.x = i - p.y * sprite->size.x;
		set_ivec2(&p, p.x + pos.x, p.y + pos.y);
		draw_point(frame, sprite->cycle[sprite->cycle_index][i], p.x, p.y);
		++i;
	}
	return ;
}
