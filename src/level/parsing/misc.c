#include "cimmerian.h"

void	remove_var_line(t_map *map, int i)
{
	free_arr((void **)map->pars.vars[i], free);
	remove_arr_elems((void **)map->pars.vars, i, i, 0);
	return ;
}

int	put_error_wall(char *tex[10][4], const char *msg, const char *arg)
{
	int	i;
	int	j;

	put_error(0, msg, arg, 0);
	i = 0;
	while (i < 10)
	{
		j = 0;
		while (j < 4)
			free(tex[i][j++]);
		++i;
	}
	return (0);
}
