#include "cimmerian.h"

static int	load_door_tex(t_man *man, t_map *map, int i, int *done);
static int	load_goal_tex(t_man *man, t_map *map, int i, int *done);

int	process_spec_wall_types(t_man *man, t_map *map)
{
	int	i;
	int	door;
	int	goal;

	i = 0;
	door = 0;
	goal = 0;
	while (map->pars.vars[i])
	{
		if (!strcmp(map->pars.vars[i][0], "D"))
		{
			map->door_line_found = 1;
			if (!load_door_tex(man, map, i, &door))
				return (0);
		}
		else if (!strcmp(map->pars.vars[i][0], "G"))
		{
			map->goal_line_found = 1;
			if (!load_goal_tex(man, map, i, &goal))
				return (0);
		}
		else
			++i;
	}
	return (1);
}

static int	load_door_tex(t_man *man, t_map *map, int i, int *done)
{
	if (*done)
		return (put_error(0, E_DUP_VAR, map->pars.vars[i][0], 0));
	*done = 1;
	if (!map->pars.vars[i][1])
		return (put_error(0, E_VAR_VAL, map->pars.vars[i][0], 0));
	else if (map->pars.vars[i][2] && map->pars.vars[i][3])
		return (put_error(0, E_VAR_VALS, map->pars.vars[i][0], 0));
	if (strcmp(map->pars.vars[i][1], "null"))
	{
		map->pars.tex_door_closed = add_image(man, map->pars.vars[i][1]);
		if (!map->pars.tex_door_closed)
			return (put_error(0, E_NO_IMG, map->pars.vars[i][1], 0));
	}
	if (map->pars.vars[i][2] && strcmp(map->pars.vars[i][2], "null"))
	{
		map->pars.tex_door_open = add_image(man, map->pars.vars[i][2]);
		if (!map->pars.tex_door_open)
			return (put_error(0, E_NO_IMG, map->pars.vars[i][2], 0));
	}
	remove_var_line(map, i);
	return (1);
}

static int	load_goal_tex(t_man *man, t_map *map, int i, int *done)
{
	if (*done)
		return (put_error(0, E_DUP_VAR, map->pars.vars[i][0], 0));
	*done = 1;
	if (!map->pars.vars[i][1])
		return (put_error(0, E_VAR_VAL, map->pars.vars[i][0], 0));
	else if (map->pars.vars[i][2])
		return (put_error(0, E_VAR_VALS, map->pars.vars[i][0], 0));
	if (strcmp(map->pars.vars[i][1], "null"))
	{
		map->pars.tex_goal = add_image(man, map->pars.vars[i][1]);
		if (!map->pars.tex_goal)
			return (put_error(0, E_NO_IMG, map->pars.vars[i][1], 0));
	}
	remove_var_line(map, i);
	return (1);
}
