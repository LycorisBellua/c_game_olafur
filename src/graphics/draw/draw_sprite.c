#include "cimmerian.h"

static void	draw_shadow(t_frame *frame, t_spr *sprite, t_ivec2 pos);

void	draw_sprite(t_frame *frame, t_spr *sprite, t_ivec2 pos)
{
	size_t	i;
	size_t	len;
	t_ivec2	p;

	if (!sprite)
		return ;
	if (sprite->cycle_shadow)
		draw_shadow(frame, sprite, pos);
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
