#include "cimmerian.h"

static int	allocate_sprites(t_map *map);
static int	set_sprite(t_man *man, t_map *map, t_spr *s, int i);

int	process_sprites(t_man *man, t_map *map)
{
	int		i;
	int		j;
	size_t	arg_len;

	if (!allocate_sprites(map))
		return (0);
	i = 0;
	j = 0;
	while (map->pars.vars[i])
	{
		if (!strcmp(map->pars.vars[i][0], "SP"))
		{
			arg_len = count_arr_elems((void **)map->pars.vars[i] + 1);
			if (arg_len < 3)
				return (put_error(0, E_VAR_VAL, map->pars.vars[i][0], 0));
			if (!set_sprite(man, map, map->sprites[j], i))
				return (0);
			++j;
			remove_var_line(map, i);
		}
		else
			++i;
	}
	return (create_audio_sources_for_sprites(map));
}

static int	allocate_sprites(t_map *map)
{
	int	i;

	i = 0;
	while (map->pars.vars[i])
	{
		if (!strcmp(map->pars.vars[i][0], "SP"))
			++map->sprite_len;
		++i;
	}
	map->sprites = calloc(map->sprite_len + 1, sizeof(t_spr *));
	if (!map->sprites)
		return (put_error(0, E_FAIL_MEM, 0, 0));
	i = 0;
	while (i < map->sprite_len)
	{
		map->sprites[i] = calloc(1, sizeof(t_spr));
		if (!map->sprites[i])
			return (put_error(0, E_FAIL_MEM, 0, 0));
		++i;
	}
	return (1);
}

static int	set_sprite(t_man *man, t_map *map, t_spr *s, int i)
{
	int	j;

	s->pos.x = get_num_dbl(map->pars.vars[i][1]);
	if (s->pos.x < 0)
		return (put_error(0, E_BAD_COORD_DBL, map->pars.vars[i][1], 0));
	s->pos.y = get_num_dbl(map->pars.vars[i][2]);
	if (s->pos.y < 0)
		return (put_error(0, E_BAD_COORD_DBL, map->pars.vars[i][2], 0));
	if (strcmp(map->pars.vars[i][3], "null"))
	{
		s->img = add_image(man, map->pars.vars[i][3]);
		if (!s->img)
			return (put_error(0, E_NO_IMG, map->pars.vars[i][3], 0));
	}
	j = 3;
	while (map->pars.vars[i][++j])
	{
		if (!strcmp(map->pars.vars[i][j], "collectible"))
			s->is_collectible = 1;
		else if (!strcmp(map->pars.vars[i][j], "collision"))
			s->has_collision = 1;
		else
			return (put_error(0, E_BAD_SP_TAG, map->pars.vars[i][j], 0));
	}
	return (1);
}
