#include "cimmerian.h"

void	find_music(t_man *man, t_map *map, char **line)
{
	char	*music_path;

	skip_whiteline(map, line);
	if (!line || !*line)
		return ;
	if (strncmp(*line, "M ", 2) == 0)
	{
		if (map->music_track)
			exit_in_parsing(man, map, E_DOUBLEMUSIC, *line);
		if (strlen(*line) < 7)
			exit_in_parsing(man, map, E_WRONGMUSIC, *line);
		else
		{
			music_path = ft_substr(*line, 2, strlen(*line) - 3);
			map->music_track = NULL;
			if (access(music_path, R_OK) == 0)
			{
				// TODO: Load music
			}
			free(music_path);
		}
	}
	free(*line);
	*line = gnl(map->fd);
}
