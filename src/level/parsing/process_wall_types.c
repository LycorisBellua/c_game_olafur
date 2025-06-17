#include "olafur.h"

static int	populate_tex(t_map *map, char *tex[10][4]);
static int	process_wall_line(t_map *map, char *tex[10][4], int i, int digit);
static int	allocate_wall_arr(t_map *map, char *tex[10][4]);
static int	fetch_images(t_man *man, char *tex[10][4], int i, t_wall_type *w);

int	process_wall_types(t_man *man, t_map *map)
{
	char	*tex[10][4];
	int		i;
	int		j;

	bzero(tex, 10 * 4 * sizeof(char *));
	if (!populate_tex(map, tex) || !allocate_wall_arr(map, tex))
		return (0);
	i = 1;
	j = 0;
	while (i < 10)
	{
		if (tex[i][0])
		{
			map->pars.tex_types_wall[j].symbol = '0' + i;
			if (!fetch_images(man, tex, i, &map->pars.tex_types_wall[j]))
			{
				free_tex_w(tex);
				return (0);
			}
			++j;
		}
		++i;
	}
	free_tex_w(tex);
	return (1);
}

static int	populate_tex(t_map *map, char *tex[10][4])
{
	int	i;
	int	digit;

	i = 0;
	while (map->pars.vars[i])
	{
		digit = !!isdigit(map->pars.vars[i][0][0]);
		if (!strcmp(map->pars.vars[i][0] + digit, "NO")
			|| !strcmp(map->pars.vars[i][0] + digit, "SO")
			|| !strcmp(map->pars.vars[i][0] + digit, "WE")
			|| !strcmp(map->pars.vars[i][0] + digit, "EA"))
		{
			if (!process_wall_line(map, tex, i, digit))
			{
				free_tex_w(tex);
				return (0);
			}
		}
		else
			++i;
	}
	return (1);
}

static int	process_wall_line(t_map *map, char *tex[10][4], int i, int digit)
{
	int	card;

	card = 0;
	if (!strcmp(map->pars.vars[i][0] + digit, "SO"))
		card = 1;
	else if (!strcmp(map->pars.vars[i][0] + digit, "WE"))
		card = 2;
	else if (!strcmp(map->pars.vars[i][0] + digit, "EA"))
		card = 3;
	if (!digit)
		digit = 1;
	else
		digit = map->pars.vars[i][0][0] - '0';
	if (!digit)
		return (put_error(0, E_TYPE_0, map->pars.vars[i][0], 0));
	else if (tex[digit][card])
		return (put_error(0, E_DUP_VAR, map->pars.vars[i][0], 0));
	else if (!map->pars.vars[i][1])
		return (put_error(0, E_VAR_VAL, map->pars.vars[i][0], 0));
	else if (map->pars.vars[i][2])
		return (put_error(0, E_VAR_VALS, map->pars.vars[i][0], 0));
	tex[digit][card] = strdup(map->pars.vars[i][1]);
	remove_var_line(map, i);
	return (1);
}

static int	allocate_wall_arr(t_map *map, char *tex[10][4])
{
	int	i;
	int	four;
	int	nbr_types;

	i = 1;
	nbr_types = 0;
	while (i < 10)
	{
		four = !!tex[i][0] + !!tex[i][1] + !!tex[i][2] + !!tex[i][3];
		if (four > 0 && four < 4)
		{
			free_tex_w(tex);
			return (put_error(0, E_CARD_NOT_FOUR, 0, 0));
		}
		++nbr_types;
		++i;
	}
	map->pars.tex_types_wall = calloc(nbr_types + 1, sizeof(t_wall_type));
	if (!map->pars.tex_types_wall)
	{
		free_tex_w(tex);
		return (put_error(0, E_FAIL_MEM, 0, 0));
	}
	return (1);
}

static int	fetch_images(t_man *man, char *tex[10][4], int i, t_wall_type *w)
{
	if (strcmp(tex[i][0], "null"))
	{
		w->tex_north = add_image(man, tex[i][0]);
		if (!w->tex_north)
			return (put_error(0, E_NO_IMG, tex[i][0], 0));
	}
	if (strcmp(tex[i][1], "null"))
	{
		w->tex_south = add_image(man, tex[i][1]);
		if (!w->tex_south)
			return (put_error(0, E_NO_IMG, tex[i][1], 0));
	}
	if (strcmp(tex[i][2], "null"))
	{
		w->tex_west = add_image(man, tex[i][2]);
		if (!w->tex_west)
			return (put_error(0, E_NO_IMG, tex[i][2], 0));
	}
	if (strcmp(tex[i][3], "null"))
	{
		w->tex_east = add_image(man, tex[i][3]);
		if (!w->tex_east)
			return (put_error(0, E_NO_IMG, tex[i][3], 0));
	}
	return (1);
}
