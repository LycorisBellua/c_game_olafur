#include "cimmerian.h"

static unsigned int	char_atoui_rgb(char *n, int *i, char sep);
static t_color		parse_color(t_man *man, t_map *map, char **line);

void	find_f_c_style(t_man *man, t_map *map, char**line, char *address)
{
	t_color	tmp;

	if (strncmp(*line, "F ", 2) == 0)
	{
		if (man->bonus && !map->types[1].tex_floor)
			map->types[1].tex_floor = add_new_image(man, address);
		else if (man->bonus)
			map->types[1].tex_f_indoor = add_new_image(man, address);
		else
		{
			tmp = parse_color(man, map, line);
			map->floor_color = get_color_rgba(tmp.r, tmp.g, tmp.b, tmp.a);
		}
	}
	else if (strncmp(*line, "C ", 2) == 0)
	{
		if (man->bonus)
			map->types[1].tex_ceiling = add_new_image(man, address);
		else
		{
			tmp = parse_color(man, map, line);
			map->ceiling_color = get_color_rgba(tmp.r, tmp.g, tmp.b, tmp.a);
		}
	}
	free(address);
}

static unsigned int	char_atoui_rgb(char *n, int *i, char sep)
{
	int	res;
	int	has_something;

	res = 0;
	has_something = 0;
	while ((n[*i]) == ' ' || n[*i] == '\t')
		(*i)++;
	while (n[*i] >= '0' && n[*i] <= '9')
	{
		has_something = 1;
		res = res * 10;
		res = res + n[(*i)] - '0';
		++(*i);
	}
	if (res < 0 || res > 255)
		return (10000);
	while ((n[*i]) == ' ' || n[*i] == '\t')
		(*i)++;
	if (n[*i] != sep || !has_something)
		return (20000);
	++(*i);
	return (res);
}

static t_color	parse_color(t_man *man, t_map *map, char **line)
{
	t_color	c;
	int		tmp[3];
	int		i;

	if ((*line)[0] == 'F' && map->floor_color.a != 0)
		exit_in_parsing(man, map, E_DOUBLEFCOL, *line);
	else if ((*line)[0] == 'C' && map->ceiling_color.a != 0)
		exit_in_parsing(man, map, E_DOUBLECCOL, *line);
	i = 1;
	while (isvalid((*line)[i], WHITES))
		i++;
	tmp[0] = char_atoui_rgb(*line, &i, ',');
	tmp[1] = char_atoui_rgb(*line, &i, ',');
	tmp[2] = char_atoui_rgb(*line, &i, '\n');
	if (tmp[0] > 255 || tmp[1] > 255 || tmp[2] > 255)
		exit_in_parsing(man, map, E_WRONGCHAR, *line);
	c.a = 255;
	c.r = tmp[0];
	c.g = tmp[1];
	c.b = tmp[2];
	return (c);
}
