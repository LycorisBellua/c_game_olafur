#include "olafur.h"

static int	allocate_door_array(t_map *map);
static int	set_door(t_man *man, t_map *map, t_door *d, int i);
static int	set_offset_and_opening_cardinal(t_map *map, t_door *d, int i,
				size_t arg_len);

int	process_doors(t_man *man, t_map *map)
{
	int	i;
	int	j;

	if (!allocate_door_array(map))
		return (0);
	else if (!map->doors)
		return (1);
	i = 0;
	j = 0;
	while (map->pars.vars[i])
	{
		if (!strcmp(map->pars.vars[i][0], "D"))
		{
			map->doors[j] = calloc(1, sizeof(t_door));
			if (!map->doors[j])
				return (put_error(0, E_FAIL_MEM, 0, 0));
			else if (!set_door(man, map, map->doors[j], i))
				return (0);
			++j;
			remove_var_line(map, i);
		}
		else
			++i;
	}
	return (1);
}

static int	allocate_door_array(t_map *map)
{
	int	i;
	int	nbr_doors;

	i = 0;
	nbr_doors = 0;
	while (map->pars.vars[i])
	{
		if (!strcmp(map->pars.vars[i][0], "D"))
			++nbr_doors;
		++i;
	}
	if (nbr_doors)
	{
		map->doors = calloc(nbr_doors + 1, sizeof(t_door *));
		if (!map->doors)
			return (put_error(0, E_FAIL_MEM, 0, 0));
	}
	return (1);
}

static int	set_door(t_man *man, t_map *map, t_door *d, int i)
{
	size_t	arg_len;

	d->m = map;
	arg_len = count_arr_elems((void **)map->pars.vars[i] + 1);
	if (arg_len < 5)
		return (put_error(0, E_VAR_VAL, map->pars.vars[i][0], 0));
	d->pos.x = get_num_int(map->pars.vars[i][1]);
	if (d->pos.x < 0)
		return (put_error(0, E_BAD_COORD, map->pars.vars[i][1], 0));
	d->pos.y = get_num_int(map->pars.vars[i][2]);
	if (d->pos.y < 0)
		return (put_error(0, E_BAD_COORD, map->pars.vars[i][2], 0));
	if (strcmp(map->pars.vars[i][3], "null"))
	{
		d->tex_closed = add_image(man, map->pars.vars[i][3]);
		if (!d->tex_closed)
			return (put_error(0, E_NO_IMG, map->pars.vars[i][3], 0));
	}
	if (strcmp(map->pars.vars[i][4], "null"))
	{
		d->tex_open = add_image(man, map->pars.vars[i][4]);
		if (!d->tex_open)
			return (put_error(0, E_NO_IMG, map->pars.vars[i][4], 0));
	}
	return (set_offset_and_opening_cardinal(map, d, i, arg_len));
}

static int	set_offset_and_opening_cardinal(t_map *map, t_door *d, int i,
	size_t arg_len)
{
	if (arg_len > 6)
		return (put_error(0, E_VAR_VALS, map->pars.vars[i][0], 0));
	d->offset = get_offset_from_str(map->pars.vars[i][5]);
	if (!d->offset)
		return (put_error(0, E_BAD_OFFSET, map->pars.vars[i][5], 0));
	if (d->offset == 'A')
		d->offset = 0;
	else
		d->is_plane = 1;
	if (arg_len == 6)
	{
		d->opening_cardinal = get_card_from_str(map->pars.vars[i][6]);
		if (!d->opening_cardinal)
			return (put_error(0, E_BAD_CARD, map->pars.vars[i][6], 0));
	}
	return (1);
}
