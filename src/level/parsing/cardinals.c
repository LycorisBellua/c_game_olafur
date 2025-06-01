#include "cimmerian.h"

static int	is_bad_map(char **line);
static int	find_cards(t_man *man, t_map *map, char **line, t_tex_type *type);
static int	find_n_wall(char *line, int *n_wall, int *n_type);

void	find_cardinals_floor_ceiling(t_man *man, t_map *map, char **line,
int n_texture)
{
	int		args;
	int		n_wall;
	int		type;

	args = 0;
	while (*line)
	{
		if ((!man->bonus && args >= 6) || (strncmp(*line, "D ", 2) == 0
				&& check_texfilled(map->types, &n_texture) == 1))
			return ;
		else if (is_bad_map(line) || n_texture == -1)
			break ;
		else if (find_n_wall(*line, &n_wall, &type) == 1)
		{
			if (n_wall > 8)
				exit_in_parsing(man, map, E_INVALIDIDTEX, *line);
			args += find_cards(man, map, line, &map->types[n_wall]);
		}
		else if (((*line)[0] == 'F' || (*line)[0] == 'C') && ++args)
			find_f_c_style(man, map, line,
				ft_substr(*line, 2, strlen(*line) - 3));
		free(*line);
		*line = gnl(map->fd);
	}
	exit_in_parsing(man, map, E_INCOMPLETE, *line);
}

int	find_cardinal_and_pos(char *line, t_ivec2 *v, char *cardinal)
{
	t_ivec2	pos;
	int		i;

	i = 0;
	skip_char(line, &i, ' ');
	if (isvalid(line[i], PLAYER) == 1)
		*cardinal = line[i++];
	else
		return (-10000);
	skip_char(line, &(i), ' ');
	if (isdigit(line[i]) == 0)
		return (-10000);
	pos.x = atoi(&line[i]);
	while (line[i] && isdigit(line[i]) == 1)
		i++;
	skip_char(line, &i, ' ');
	if (isdigit(line[i]) == 0)
		return (-10000);
	pos.y = atoi(line + i);
	while (line[i] && isdigit(line[i]) == 1)
		i++;
	set_ivec2(v, pos.x, pos.y);
	return (i);
}

static int	is_bad_map(char **line)
{
	int	digit;

	if (onlyvalids(*line, WHITES) == 0)
	{
		digit = isdigit(*line[0]);
		if (strlen(*line) < 7)
			return (1);
		if (strncmp(*line, "F", 1) == 0 || strncmp(*line, "C", 1) == 0
			|| strncmp(*line + digit, "SO", 2) == 0
			|| strncmp(*line + digit, "WE", 2) == 0
			|| strncmp(*line + digit, "EA", 2) == 0
			|| strncmp(*line + digit, "NO", 2) == 0)
			return (0);
		else
			return (1);
	}
	return (0);
}

static int	find_cards(t_man *man, t_map *map, char **line, t_tex_type *type)
{
	int		tmp;
	int		n;
	char	*path;

	find_n_wall(*line, &tmp, &n);
	tmp = 0;
	path = ft_substr(*line + n, 3, strlen(*line + n) - 4);
	if (check_doublewall(*line + n, type) == -1)
		exit_in_parsing(man, map, E_DOUBLEWALL, *line);
	if (strncmp(*line + n, "NO", 2) == 0 && ++tmp)
		type->tex_north = add_new_image(man, path);
	else if (strncmp(*line + n, "SO", 2) == 0 && ++tmp)
		type->tex_south = add_new_image(man, path);
	else if (strncmp(*line + n, "EA", 2) == 0 && ++tmp)
		type->tex_east = add_new_image(man, path);
	else if (strncmp(*line + n, "WE", 2) == 0 && ++tmp)
		type->tex_west = add_new_image(man, path);
	return (free(path), tmp);
}

static int	find_n_wall(char *line, int *n_wall, int *n_type)
{
	*n_wall = 1;
	*n_type = 0;
	if (strncmp(line, "NO", 2) == 0 || strncmp(line, "SO", 2) == 0
		|| strncmp(line, "WE", 2) == 0 || strncmp(line, "EA", 2) == 0)
		return (1);
	else if (isdigit(line[0]) == 1)
	{
		*n_wall = atoi(line);
		*n_type = 1;
		if (strncmp(line + 1, "NO", 2) == 0
			|| strncmp(line + 1, "SO", 2) == 0
			|| strncmp(line + 1, "WE", 2) == 0
			|| strncmp(line + 1, "EA", 2) == 0)
			return (1);
	}
	return (0);
}
