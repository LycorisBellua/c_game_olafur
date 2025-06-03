#include "cimmerian.h"

void	find_player(t_man *man, t_map *map, char *s, int y)
{
	int	x;

	x = 0;
	if (!s)
		return ;
	while (s[x])
	{
		if (isvalid(s[x], PLAYER))
		{
			if (map->start_pos.x != -1)
				exit_in_parsing(man, map, E_DOUBLEPLAYER, NULL);
			set_vec2(&map->start_pos, x + 0.5, y + 0.5);
			map->start_dir = get_dir_from_cardinal(s[x]);
			s[x] = '0';
		}
		x++;
	}
}

void	find_door_goal(t_man *man, t_map *map, char **line)
{
	char	*path;

	skip_whiteline(map, line);
	if (!strncmp(*line, "D ", 2))
	{
		path = ft_substr(*line, 2, strlen(*line) - 3);
		map->types[1].tex_door_closed = add_new_image(man, path);
		map->types[1].tex_door_open = state_open(man, path);
		free(path);
		free(*line);
		*line = gnl(map->fd);
	}
	if (!strncmp(*line, "G ", 2))
	{
		path = ft_substr(*line, 2, strlen(*line) - 3);
		map->types[1].tex_goal = add_new_image(man, path);
		free(path);
	}
	free(*line);
	*line = gnl(map->fd);
}

t_img	*state_open(t_man *man, char *path)
{
	t_img	*open;
	char	*compose_path;
	int		size;

	open = NULL;
	size = strlen(path);
	if (size < 5)
		return (NULL);
	compose_path = calloc(size + 5 + 1, sizeof(char));
	if (!compose_path)
		return (NULL);
	ft_strncpy(compose_path, path, size - 4);
	ft_strncpy(compose_path + (size - 4), "_open", 5);
	ft_strncpy(compose_path + (size - 4 + 5), ".png", 4);
	if (!access(compose_path, R_OK))
		open = add_new_image(man, compose_path);
	free(compose_path);
	return (open);
}

void	find_skybox(t_man *man, t_map *map, char **line)
{
	t_img	*src_skybox;
	char	*path;

	skip_whiteline(map, line);
	path = ft_substr(*line, 2, strlen(*line) - 3);
	if (!strncmp(*line, "S ", 2))
	{
		if (map->skybox)
			exit_in_parsing(man, map, E_DOUBLESKYBOX, *line);
		src_skybox = add_new_image(man, path);
		free(path);
		if (src_skybox)
			map->fog_color = src_skybox->average_color[0];
		else
		{
			map->ceiling_color.a = 255;
			map->floor_color.a = 255;
		}
		compose_skybox(man, map, src_skybox);
	}
	free(*line);
	*line = gnl(map->fd);
}
