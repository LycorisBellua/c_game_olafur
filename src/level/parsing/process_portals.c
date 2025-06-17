#include "olafur.h"

static int	allocate_portal_array(t_map *map);
static int	set_portal(t_man *man, t_map *map, t_portal *p, int i);
static int	set_portal_src(t_map *map, t_portal *p, int i);
static int	set_portal_tex_and_dst_path(t_man *man, t_map *map, t_portal *p,
				int i);

int	process_portals(t_man *man, t_map *map)
{
	int	i;
	int	j;

	if (!allocate_portal_array(map))
		return (0);
	else if (!map->portals)
		return (1);
	i = 0;
	j = 0;
	while (map->pars.vars[i])
	{
		if (!strcmp(map->pars.vars[i][0], "PT"))
		{
			map->portals[j] = calloc(1, sizeof(t_portal));
			if (!map->portals[j])
				return (put_error(0, E_FAIL_MEM, 0, 0));
			else if (!set_portal(man, map, map->portals[j], i))
				return (0);
			++j;
			remove_var_line(map, i);
		}
		else
			++i;
	}
	return (1);
}

static int	allocate_portal_array(t_map *map)
{
	int	i;
	int	nbr_portals;

	i = 0;
	nbr_portals = 0;
	while (map->pars.vars[i])
	{
		if (!strcmp(map->pars.vars[i][0], "PT"))
			++nbr_portals;
		++i;
	}
	if (nbr_portals)
	{
		map->portals = calloc(nbr_portals + 1, sizeof(t_portal *));
		if (!map->portals)
			return (put_error(0, E_FAIL_MEM, 0, 0));
	}
	return (1);
}

static int	set_portal(t_man *man, t_map *map, t_portal *p, int i)
{
	size_t	arg_len;

	p->src_map = map;
	p->is_corridor = PORTAL_IS_CORRIDOR;
	arg_len = count_arr_elems((void **)map->pars.vars[i] + 1);
	if (arg_len != 6 && arg_len != 9)
		return (put_error(0, E_VAR_VAL, map->pars.vars[i][0], 0));
	if (arg_len == 9)
	{
		p->override_start_pos = 1;
		p->dst_cardinal = get_card_from_str(map->pars.vars[i][7]);
		if (!p->dst_cardinal)
			return (put_error(0, E_BAD_CARD, map->pars.vars[i][7], 0));
		p->dst_pos.x = get_num_int(map->pars.vars[i][8]);
		if (p->dst_pos.x < 0)
			return (put_error(0, E_BAD_COORD, map->pars.vars[i][8], 0));
		p->dst_pos.y = get_num_int(map->pars.vars[i][9]);
		if (p->dst_pos.y < 0)
			return (put_error(0, E_BAD_COORD, map->pars.vars[i][9], 0));
	}
	return (set_portal_src(map, p, i)
		&& set_portal_tex_and_dst_path(man, map, p, i));
}

static int	set_portal_src(t_map *map, t_portal *p, int i)
{
	p->src_cardinal = get_card_from_str(map->pars.vars[i][1]);
	if (!p->src_cardinal)
		return (put_error(0, E_BAD_CARD, map->pars.vars[i][1], 0));
	p->src_pos.x = get_num_int(map->pars.vars[i][2]);
	if (p->src_pos.x < 0)
		return (put_error(0, E_BAD_COORD, map->pars.vars[i][2], 0));
	p->src_pos.y = get_num_int(map->pars.vars[i][3]);
	if (p->src_pos.y < 0)
		return (put_error(0, E_BAD_COORD, map->pars.vars[i][3], 0));
	return (1);
}

static int	set_portal_tex_and_dst_path(t_man *man, t_map *map, t_portal *p,
	int i)
{
	if (strcmp(map->pars.vars[i][4], "null"))
	{
		p->tex_closed = add_image(man, map->pars.vars[i][4]);
		if (!p->tex_closed)
			return (put_error(0, E_NO_IMG, map->pars.vars[i][4], 0));
	}
	if (strcmp(map->pars.vars[i][5], "null"))
	{
		p->tex_open = add_image(man, map->pars.vars[i][5]);
		if (!p->tex_open)
			return (put_error(0, E_NO_IMG, map->pars.vars[i][5], 0));
	}
	if (strcmp(map->pars.vars[i][6], "null"))
	{
		p->path_dst_map = strdup(map->pars.vars[i][6]);
		if (!p->path_dst_map)
			return (put_error(0, E_FAIL_MEM, 0, 0));
	}
	return (1);
}
