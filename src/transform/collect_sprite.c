#include "cimmerian.h"

static int		collect_sprite(t_player *player, t_map *map, int sprite_index);
static t_vec2	calculate_new_pos_on_collision(t_spr *s, double radius,
					t_vec2 pos);

void	adjust_position_on_sprite_collision(t_man *man, t_map *map)
{
	int		i;
	t_vec2	pos;
	double	radius;

	pos = man->player.pos;
	radius = man->player.radius + SPRITE_RADIUS;
	i = 0;
	while (i < map->sprite_len)
	{
		if (map->sprites[i]->dist < radius)
		{
			if (collect_sprite(&man->player, map, i))
				continue ;
			else if (map->sprites[i]->has_collision)
				pos = calculate_new_pos_on_collision(map->sprites[i], radius,
					pos);
		}
		++i;
	}
	man->player.pos = pos;
	return ;
}

static int	collect_sprite(t_player *player, t_map *map, int sprite_index)
{
	int	i;

	if (!map->sprites[sprite_index]->is_collectible
		|| player->collected >= player->to_collect)
		return (0);
	free(map->sprites[sprite_index]);
	i = sprite_index;
	while (i < map->sprite_len - 1)
	{
		map->sprites[i] = map->sprites[i + 1];
		++i;
	}
	--map->sprite_len;
	++player->collected;
	return (1);
}

static t_vec2	calculate_new_pos_on_collision(t_spr *s, double radius,
	t_vec2 pos)
{
	double	overlap;
	t_vec2	push;

	overlap = radius - s->dist;
	push.x = (pos.x - s->pos.x) / s->dist * overlap;
	push.y = (pos.y - s->pos.y) / s->dist * overlap;
	set_vec2(&pos, pos.x + push.x, pos.y + push.y);
	return (pos);
}
