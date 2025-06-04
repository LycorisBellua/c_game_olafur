#include "cimmerian.h"

/*
static char		*get_portals(t_map *map, char **line);
static t_portal	**set_portals(t_man *man, t_map *map, char *line);
static t_portal	*valid_portal(t_man *man, t_map *map, char *line, int i);
static char		*get_portal_absolute_path(char *path);

void	find_portals(t_man *man, t_map *map, char **line)
{
	char	*line_portals;

	line_portals = get_portals(map, line);
	map->portals = set_portals(man, map, line_portals);
	free(line_portals);
}

static char	*get_portals(t_map *map, char **line)
{
	char	*portals;
	char	*tmp;

	portals = calloc(1, sizeof(char));
	while (*line)
	{
		if (!strncmp(*line, "P", 1))
		{
			tmp = strjoin(portals, *line);
			free(portals);
			portals = tmp;
			map->portal_len++;
		}
		else
		{
			if (!onlyvalids(*line, WHITES))
				break ;
		}
		free(*line);
		*line = gnl(map->fd);
	}
	return (portals);
}

static t_portal	**set_portals(t_man *man, t_map *map, char *line)
{
	t_portal	**portals;
	char		*tmp;
	int			i;
	int			start;
	int			end;

	tmp = NULL;
	i = 0;
	portals = calloc(map->portal_len, sizeof(t_portal));
	start = 0;
	end = start;
	while (i < map->portal_len)
	{
		if (line[end] == '\n' || line[end] == '\0')
		{
			tmp = ft_substr(line, start, end - start);
			portals[i] = valid_portal(man, map, tmp + 1, 0);
			free(tmp);
			start = end + 1;
			i++;
		}
		++end;
	}
	return (portals);
}

static t_portal	*valid_portal(t_man *man, t_map *map, char *line, int i)
{
	t_portal	*ptl;
	int			size;
	char		*path;

	ptl = calloc(1, sizeof(t_portal));
	if (!ptl)
		return (NULL);
	ptl->src_map = map;
	ptl->is_corridor = PORTAL_IS_CORRIDOR;
	i += find_cardinal_and_pos(line, &ptl->src_pos, &ptl->src_cardinal);
	if (i < 0)
		exit_in_parsing(man, map, E_WRONGCHAR, ptl);
	i += find_text(line + i, &size);
	path = ft_substr(line, i - size, size);
	ptl->tex_closed = add_new_image(man, path);
	ptl->tex_open = state_open(man, path);
	free(path);
	i += find_text(line + i, &size);
	ptl->path_dst_map = get_portal_absolute_path(ft_substr(line, i - size, size));
	ptl->override_start_pos = 1;
	i += find_cardinal_and_pos(line + i, &ptl->dst_pos, &ptl->dst_cardinal);
	if (i < 0 || !onlyvalids(line + i, WHITES))
		return (free(ptl->path_dst_map),
			exit_in_parsing(man, map, E_WRONGCHAR, ptl), NULL);
	return (ptl);
}

static char	*get_portal_absolute_path(char *path)
{
	char	*absolute;

	if (!strcmp(path, "null"))
	{
		free(path);
		return (NULL);
	}
	absolute = get_absolute_path(path);
	free(path);
	return (absolute);
}
*/
