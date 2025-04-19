#include "cimmerian.h"

static void	set_start_and_end_indexes(char **lines, size_t *start, size_t *end);
static void	set_player_start(t_map *m);

int	extract_maps_and_player_start(t_map *m)
{
	char	**lines;
	size_t	start;
	size_t	end;

	lines = read_file_lines(m->filepath);
	if (!lines)
		return (0);
	end = 0;
	set_start_and_end_indexes(lines, &start, &end);
	// is this strlen safe? (`lines` exists but what about `lines[start]`)
	set_ivec2(&m->size, strlen(lines[start]), end - start);
	m->map_walls = strjoin_arr(lines, start, end);
	if (!m->map_walls)
	{
		free_arr((void **)lines, free);
		return (0);
	}
	set_player_start(m);
	set_start_and_end_indexes(lines, &start, &end);
	if (start < end)
		m->map_ceil_floor = strjoin_arr(lines, start, end);
	else
		m->map_ceil_floor = calloc(m->size.x * m->size.y, sizeof(char));
	free_arr((void **)lines, free);
	if (!m->map_ceil_floor)
	{
		free(m->map_walls);
		m->map_walls = 0;
	}
	return (!!m->map_ceil_floor);
}

static void	set_start_and_end_indexes(char **lines, size_t *start, size_t *end)
{
	if (!lines)
		return ;
	*start = *end;
	while (lines[*start] && strncmp(lines[*start], "map_", 4))
		++*start;
	if (lines[*start])
		++*start;
	*end = *start;
	while (lines[*end] && strncmp(lines[*end], "map_", 4)
		&& strncmp(lines[*end], "sprite_", 7))
		++*end;
	return ;
}

static void	set_player_start(t_map *m)
{
	t_ivec2	coord;

	coord.x = 0;
	while (coord.x < m->size.x)
	{
		coord.y = 0;
		while (coord.y < m->size.y)
		{
			if (m->map_walls[coord.y * m->size.x + coord.x] == 'N'
				|| m->map_walls[coord.y * m->size.x + coord.x] == 'S'
				|| m->map_walls[coord.y * m->size.x + coord.x] == 'W'
				|| m->map_walls[coord.y * m->size.x + coord.x] == 'E')
			{
				set_vec2(&m->start_pos, coord.x, coord.y);
				m->start_dir = get_dir_from_cardinal(m->map_walls[coord.y
						* m->size.x + coord.x]);
				return ;
			}
			++coord.y;
		}
		++coord.x;
	}
	return ;
}
