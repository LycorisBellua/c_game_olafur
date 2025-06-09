#include "cimmerian.h"

static int	populate_tex(t_map *map, char **tex);
static int	process_floor_line(t_map *map, char **tex, int i);
static int	allocate_floor_arr(t_map *map, char **tex);
static int	fetch_images(t_man *man, char **tex, int i, t_row_type *f);

int	process_floor_types(t_man *man, t_map *map)
{
	char	*tex[10];
	int		i;
	int		j;

	bzero(tex, 10 * sizeof(char *));
	if (!populate_tex(map, tex) || !allocate_floor_arr(map, tex))
	{
		free_tex_fc(tex);
		return (0);
	}
	i = 1;
	j = 0;
	while (i < 10)
	{
		if (tex[i])
		{
			map->pars.tex_types_floor[j].symbol = '0' + i;
			if (!fetch_images(man, tex, i, &map->pars.tex_types_floor[j]))
				return (0);
			++j;
		}
		++i;
	}
	free_tex_fc(tex);
	return (1);
}

static int	populate_tex(t_map *map, char **tex)
{
	int	i;

	i = 0;
	while (map->pars.vars[i])
	{
		if (isdigit(map->pars.vars[i][0][0])
			&& !strcmp(map->pars.vars[i][0] + 1, "F"))
		{
			if (!process_floor_line(map, tex, i))
				return (0);
		}
		else
			++i;
	}
	return (1);
}

static int	process_floor_line(t_map *map, char **tex, int i)
{
	int	digit;

	digit = map->pars.vars[i][0][0] - '0';
	if (!digit)
		return (put_error(0, E_TYPE_0, map->pars.vars[i][0], 0));
	else if (tex[digit])
		return (put_error(0, E_DUP_VAR, map->pars.vars[i][0], 0));
	else if (!map->pars.vars[i][1])
		return (put_error(0, E_VAR_VAL, map->pars.vars[i][0], 0));
	else if (map->pars.vars[i][2])
		return (put_error(0, E_VAR_VALS, map->pars.vars[i][0], 0));
	tex[digit] = strdup(map->pars.vars[i][1]);
	remove_var_line(map, i);
	return (1);
}

static int	allocate_floor_arr(t_map *map, char **tex)
{
	int	i;
	int	nbr_types;

	i = 1;
	nbr_types = 0;
	while (i < 10)
	{
		nbr_types += !!tex[i];
		++i;
	}
	map->pars.tex_types_floor = calloc(nbr_types + 1, sizeof(t_row_type));
	if (!map->pars.tex_types_floor)
		return (put_error(0, E_FAIL_MEM, 0, 0));
	return (1);
}

static int	fetch_images(t_man *man, char **tex, int i, t_row_type *f)
{
	if (strcmp(tex[i], "null"))
	{
		f->tex = add_image(man, tex[i]);
		if (!f->tex)
		{
			put_error(0, E_NO_IMG, tex[i], 0);
			free_tex_fc(tex);
			return (0);
		}
	}
	return (1);
}
