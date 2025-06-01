#include "cimmerian.h"

static char	*get_sprites(t_map *map, char **line);
static void	set_sprite(t_spr *spr, t_img *img, t_ivec2 col_light, t_vec2 pos);
static int	set_sprites(t_man *man, t_map *map, char *line);
static int	allocate_sprites(t_map *map);

void	find_sprites(t_man *man, t_map *map, char **line)
{
	char	*line_sprites;

	line_sprites = NULL;
	line_sprites = get_sprites(map, line);
	if (map->sprite_len > 10000)
	{
		free(*line);
		free(line_sprites);
		exit_in_parsing(man, map, E_NBSPRITES, NULL);
	}
	if (!allocate_sprites(map))
	{
		free(*line);
		free(line_sprites);
		exit_in_parsing(man, map, E_WRONGCHAR, NULL);
	}
	if (set_sprites(man, map, line_sprites) == 0)
	{
		free(*line);
		free(line_sprites);
		exit_in_parsing(man, map, E_WRONGCHAR, NULL);
	}
	free(line_sprites);
}

static char	*get_sprites(t_map *map, char **line)
{
	char	*sprite;

	sprite = calloc(1, sizeof(char));
	while (*line)
	{
		if (strncmp(*line, "sprite_", 7) == 0)
		{
			append(&sprite, *line);
			*line = gnl(map->fd);
			while (*line && isdigit((*line)[skip_space(*line)]) == 1)
			{
				map->sprite_len++;
				append(&sprite, *line);
				*line = gnl(map->fd);
			}
		}
		else if (onlyvalids(*line, WHITES) == 0)
			break ;
		free(*line);
		*line = gnl(map->fd);
	}
	return (sprite);
}

static void	set_sprite(t_spr *spr, t_img *img, t_ivec2 col_light, t_vec2 pos)
{
	spr->img = img;
	spr->is_collectible = col_light.x;
	spr->has_collision = !col_light.y;
	set_vec2(&spr->pos, pos.x, pos.y);
}

static int	set_sprites(t_man *man, t_map *map, char *line)
{
	char	*id;
	int		sp;
	int		start;
	t_vec2	pos;
	t_ivec2	spec;

	start = 0;
	sp = 0;
	id = NULL;
	while (sp < map->sprite_len)
	{
		if (strncmp(line + start, "sprite_", 7) == 0)
		{
			free(id);
			if (get_id(line + start, &start, &spec, &id) == -1)
				return (free(id), 0);
		}
		else if (isdigit(line[skip_char(line, &start, ' ')]) == 1)
		{
			if (get_pos(line + start, &start, &pos) == -1)
				return (free(id), 0);
			set_sprite(map->sprites[sp++], add_new_image(man, id), spec, pos);
		}
	}
	return (free(id), 1);
}

static int	allocate_sprites(t_map *map)
{
	int	i;

	if (map->sprites)
	{
		free_sprite_array(map);
		return (0);
	}
	map->sprites = calloc(map->sprite_len, sizeof(t_spr *));
	if (!map->sprites && map->sprite_len != 0)
	{
		free_sprite_array(map);
		return (0);
	}
	i = -1;
	while (++i < map->sprite_len)
	{
		map->sprites[i] = calloc(1, sizeof(t_spr));
		if (!map->sprites[i])
		{
			free_sprite_array(map);
			return (0);
		}
	}
	return (1);
}
