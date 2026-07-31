#include "olafur.h"

static int	set_sequence(t_map *map, int i);
static int	is_digit_or_upper(int c);

int	process_teleport(t_map *map)
{
	int		i;
	size_t	arg_len;

	i = 0;
	while (map->pars.vars[i])
	{
		if (!strcmp(map->pars.vars[i][0], "TP"))
		{
			if (map->tp.map_path)
				return (put_error(0, E_DUP_VAR, map->pars.vars[i][0], 0));
			arg_len = count_arr_elems((void **)map->pars.vars[i] + 1);
			if (arg_len < 3)
				return (put_error(0, E_VAR_VAL, map->pars.vars[i][0], 0));
			free(map->tp.map_path);
			free(map->tp.sequence);
			map->tp.map_path = strdup(map->pars.vars[i][1]);
			map->tp.seq_len = arg_len - 1;
			if (!set_sequence(map, i))
				return (0);
			remove_var_line(map, i);
		}
		else
			++i;
	}
	return (1);
}

static int	set_sequence(t_map *map, int i)
{
	int	j;

	map->tp.sequence = calloc(map->tp.seq_len, sizeof(int));
	if (!map->tp.sequence)
		return (put_error(0, E_FAIL_MEM, map->pars.vars[i][0], 0));
	j = -1;
	while (++j < map->tp.seq_len)
	{
		if (!strcmp(map->pars.vars[i][2 + j], "ENTER"))
			map->tp.sequence[j] = GLFW_KEY_ENTER;
		else if (!strcmp(map->pars.vars[i][2 + j], "UP"))
			map->tp.sequence[j] = GLFW_KEY_UP;
		else if (!strcmp(map->pars.vars[i][2 + j], "DOWN"))
			map->tp.sequence[j] = GLFW_KEY_DOWN;
		else if (!strcmp(map->pars.vars[i][2 + j], "LEFT"))
			map->tp.sequence[j] = GLFW_KEY_LEFT;
		else if (!strcmp(map->pars.vars[i][2 + j], "RIGHT"))
			map->tp.sequence[j] = GLFW_KEY_RIGHT;
		else if (strlen(map->pars.vars[i][2 + j]) == 1
			&& is_digit_or_upper(map->pars.vars[i][2 + j][0]))
			map->tp.sequence[j] = map->pars.vars[i][2 + j][0];
		else
			return (put_error(0, E_BAD_KEY, map->pars.vars[i][0], 0));
	}
	return (1);
}

static int	is_digit_or_upper(int c)
{
	return (isdigit(c) || isupper(c));
}
