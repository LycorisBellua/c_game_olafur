#include "cimmerian.h"

static int	count_sprites(char **lines);

static void	set_soul_sprites(t_spr **spr, t_img *img);
static void	set_pillar_sprites(t_spr **spr, t_img *img);
static void	set_barrel_sprites(t_spr **spr, t_img *img);

int	extract_sprites(t_man *man, t_map *map)
{
	char	**lines;

	lines = read_file_lines(map->filepath);
	if (!lines)
		return (0);
	map->sprite_len = count_sprites(lines);
	if (!map->sprite_len)
	{
		free_arr((void **)lines, free);
		return (1);
	}
	map->sprites = calloc(map->sprite_len, sizeof(t_spr *));
	if (!map->sprites)
	{
		free_arr((void **)lines, free);
		return (0);
	}

	/*
sprite_soul_idle:
20.5, 11.5
18.5,  4.5
10.0,  4.5
10.0, 12.5
 3.5,  6.5
 3.5, 20.5
 3.5, 14.5
14.5, 20.5

sprite_pillar:
18.5, 10.5
18.5, 11.5
18.5, 12.5
 8.5,  7.0

sprite_barrel:
21.5,  1.5
15.5,  1.5
16.0,  1.8
16.2,  1.2
 3.5,  2.5
 9.5, 15.5
10.0, 15.1
10.5, 15.8
	*/

	int		i;
	i = 0;
	while (i < map->sprite_len)
	{
		map->sprites[i] = calloc(1, sizeof(t_spr));
		if (!map->sprites[i])
		{
			free_arr((void **)lines, free);
			return (0);
		}
		++i;
	}
	set_soul_sprites(map->sprites, get_image(man, "soul_idle"));
	set_pillar_sprites(map->sprites, get_image(man, "pillar"));
	set_barrel_sprites(map->sprites, get_image(man, "barrel"));
	free_arr((void **)lines, free);
	return (1);
}

void	free_sprite_array(t_map *map)
{
	int	i;

	if (!map)
		return ;
	if (map->sprites)
	{
		i = 0;
		while (i < map->sprite_len)
		{
			free(map->sprites[i]);
			++i;
		}
	}
	free(map->sprites);
	map->sprites = 0;
	map->sprite_len = 0;
	return ;
}

static int	count_sprites(char **lines)
{
	int	i;
	int	count;

	if (!lines)
		return (0);
	i = 0;
	count = 0;
	while (lines[i])
	{
		while (lines[i] && strncmp(lines[i], "sprite_", 7))
			++i;
		if (!lines[i++])
			break ;
		while (lines[i] && strncmp(lines[i], "map_", 4)
			&& strncmp(lines[i], "sprite_", 7))
		{
			++i;
			++count;
		}
	}
	return (count);
}

static void	set_soul_sprites(t_spr **spr, t_img *img)
{
	int	i;

	i = 0;
	while (i < 8)
	{
		spr[i]->img = img;
		++i;
	}
	set_vec2(&spr[0]->pos, 20.5, 11.5);
	set_vec2(&spr[1]->pos, 18.5, 4.5);
	set_vec2(&spr[2]->pos, 10.0, 4.5);
	set_vec2(&spr[3]->pos, 10.0, 12.5);
	set_vec2(&spr[4]->pos, 3.5, 6.5);
	set_vec2(&spr[5]->pos, 3.5, 20.5);
	set_vec2(&spr[6]->pos, 3.5, 14.5);
	set_vec2(&spr[7]->pos, 14.5, 20.5);
	if (img && !strcmp(img->id, SPRITE_TYPE_TO_COLLECT))
	{
		spr[0]->is_collectible = 1;
		spr[1]->is_collectible = 1;
		spr[2]->is_collectible = 1;
		spr[3]->is_collectible = 1;
		spr[4]->is_collectible = 1;
		spr[5]->is_collectible = 1;
		spr[6]->is_collectible = 1;
		spr[7]->is_collectible = 1;
	}
	return ;
}

static void	set_pillar_sprites(t_spr **spr, t_img *img)
{
	int	i;

	i = 8;
	while (i < 12)
	{
		spr[i]->img = img;
		++i;
	}
	set_vec2(&spr[8]->pos, 18.5, 10.5);
	set_vec2(&spr[9]->pos, 18.5, 11.5);
	set_vec2(&spr[10]->pos, 18.5, 12.5);
	set_vec2(&spr[11]->pos, 8.5, 7.0);
	return ;
}

static void	set_barrel_sprites(t_spr **spr, t_img *img)
{
	int	i;

	i = 12;
	while (i < 20)
	{
		spr[i]->img = img;
		++i;
	}
	set_vec2(&spr[12]->pos, 21.5, 1.5);
	set_vec2(&spr[13]->pos, 15.5, 1.5);
	set_vec2(&spr[14]->pos, 16.0, 1.8);
	set_vec2(&spr[15]->pos, 16.2, 1.2);
	set_vec2(&spr[16]->pos, 3.5, 2.5);
	set_vec2(&spr[17]->pos, 9.5, 15.5);
	set_vec2(&spr[18]->pos, 10.0, 15.1);
	set_vec2(&spr[19]->pos, 10.5, 15.8);
	return ;
}
