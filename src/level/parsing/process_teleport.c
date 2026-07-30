#include "olafur.h"

int	process_teleport(t_map *map)
{
	int		i;
	size_t	arg_len;

	i = 0;
	while (map->pars.vars[i])
	{
		if (!strcmp(map->pars.vars[i][0], "TP"))
		{
			arg_len = count_arr_elems((void **)map->pars.vars[i] + 1);
			if (arg_len == 1)
			{
				free(map->tp_path_map);
				map->tp_path_map = strdup(map->pars.vars[i][1]);
				remove_var_line(map, i);
			}
			else if (!arg_len)
				return (put_error(0, E_VAR_VAL, map->pars.vars[i][0], 0));
			else
				return (put_error(0, E_VAR_VALS, map->pars.vars[i][0], 0));
		}
		else
			++i;
	}
	return (1);
}
