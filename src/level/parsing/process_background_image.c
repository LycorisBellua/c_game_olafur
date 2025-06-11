#include "cimmerian.h"

static int	process_floor_ceil_colors(t_map *map);
static int	load_color(t_map *map, int i, t_color *color);
static int	load_skybox(t_man *man, t_map *map, t_img **skybox);

int	process_background_image(t_man *man, t_map *map)
{
	t_img	*skybox;

	skybox = 0;
	if (!process_floor_ceil_colors(map) || !load_skybox(man, map, &skybox))
		return (0);
	if (!skybox)
	{
		map->floor_color.a = 255;
		map->ceil_color.a = 255;
	}
	compose_skybox(man, map, skybox);
	compose_background(man, map);
	return (1);
}

static int	process_floor_ceil_colors(t_map *map)
{
	int	i;

	i = 0;
	while (map->pars.vars[i])
	{
		if (!strcmp(map->pars.vars[i][0], "F"))
		{
			if (map->floor_color.a)
				return (put_error(0, E_DUP_VAR, map->pars.vars[i][0], 0));
			else if (!load_color(map, i, &map->floor_color))
				return (0);
			remove_var_line(map, i);
		}
		else if (!strcmp(map->pars.vars[i][0], "C"))
		{
			if (map->ceil_color.a)
				return (put_error(0, E_DUP_VAR, map->pars.vars[i][0], 0));
			else if (!load_color(map, i, &map->ceil_color))
				return (0);
			remove_var_line(map, i);
		}
		else
			++i;
	}
	return (1);
}

static int	load_color(t_map *map, int i, t_color *color)
{
	size_t	arg_len;
	int		rgb[3];

	arg_len = count_arr_elems((void **)map->pars.vars[i] + 1);
	if (!arg_len)
		return (put_error(0, E_VAR_VAL, map->pars.vars[i][0], 0));
	else if (arg_len == 1)
	{
		*color = get_color_hex(map->pars.vars[i][1], 255);
		if (!color->a)
			return (put_error(0, E_BAD_COLOR, map->pars.vars[i][0], 0));
	}
	else if (arg_len == 3)
	{
		rgb[0] = get_num_rgb(map->pars.vars[i][1]);
		rgb[1] = get_num_rgb(map->pars.vars[i][2]);
		rgb[2] = get_num_rgb(map->pars.vars[i][3]);
		if (rgb[0] < 0 || rgb[1] < 0 || rgb[2] < 0)
			return (put_error(0, E_BAD_COLOR, map->pars.vars[i][0], 0));
		*color = get_color_rgba(rgb[0], rgb[1], rgb[2], 255);
	}
	else
		return (put_error(0, E_BAD_COLOR, map->pars.vars[i][0], 0));
	return (1);
}

static int	load_skybox(t_man *man, t_map *map, t_img **skybox)
{
	int	i;

	i = 0;
	while (map->pars.vars[i])
	{
		if (!strcmp(map->pars.vars[i][0], "SK"))
		{
			if (*skybox)
				return (put_error(0, E_DUP_VAR, map->pars.vars[i][0], 0));
			else if (!map->pars.vars[i][1])
				return (put_error(0, E_VAR_VAL, map->pars.vars[i][0], 0));
			else if (map->pars.vars[i][2])
				return (put_error(0, E_VAR_VALS, map->pars.vars[i][0], 0));
			else if (strcmp(map->pars.vars[i][1], "null"))
				*skybox = add_image(man, map->pars.vars[i][1]);
			remove_var_line(map, i);
		}
		else
			++i;
	}
	return (1);
}
