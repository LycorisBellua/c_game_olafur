#include "cimmerian.h"

void	remove_var_line(t_map *map, int i)
{
	free_arr((void **)map->pars.vars[i], free);
	remove_arr_elems((void **)map->pars.vars, i, i, 0);
	return ;
}

int	is_var_list_empty(t_map *map)
{
	if (map->pars.vars[0])
		return (put_error(0, E_BAD_VAR, map->pars.vars[0][0], 0));
	return (1);
}

void	free_tex_w(char *tex[10][4])
{
	int	i;
	int	j;

	i = 0;
	while (i < 10)
	{
		j = 0;
		while (j < 4)
			free(tex[i][j++]);
		++i;
	}
	return ;
}

void	free_tex_fc(char *tex[10])
{
	int	i;

	i = 0;
	while (i < 10)
	{
		free(tex[i]);
		++i;
	}
	return ;
}
