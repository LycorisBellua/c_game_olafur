#include "cimmerian.h"

void	release_parsing_data(t_map *map)
{
	int	i;

	if (!map)
		return ;
	free_arr((void **)map->pars.lines, free);
	if (map->pars.vars)
	{
		i = -1;
		while (map->pars.vars[++i])
			free_arr((void **)map->pars.vars[i], free);
		free(map->pars.vars);
	}
	free_arr((void **)map->pars.map_wall, free);
	free_arr((void **)map->pars.map_floor, free);
	free_arr((void **)map->pars.map_ceil, free);
	free(map->pars.tex_types_wall);
	free(map->pars.tex_types_floor);
	free(map->pars.tex_types_ceil);
	bzero(&map->pars, sizeof(t_map_pars));
	return ;
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
