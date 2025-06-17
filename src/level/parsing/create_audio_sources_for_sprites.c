#include "olafur.h"

static int	allocate_source_array(t_map *map);

int	create_audio_sources_for_sprites(t_map *map)
{
	int			i_sprite;
	int			i_source;
	t_a_track	*t;

	if (!allocate_source_array(map))
		return (0);
	else if (!map->sources)
		return (1);
	t = map->tracks[SOUND_COLLEC];
	i_sprite = -1;
	i_source = -1;
	while (map->sprites[++i_sprite])
	{
		if (map->sprites[i_sprite]->is_collectible)
		{
			map->sources[++i_source] = audio_source_create(t, 0);
			if (!map->sources[i_source])
				return (put_error(0, E_FAIL_MEM, 0, 0));
			map->sprites[i_sprite]->source_collec = map->sources[i_source];
			audio_source_spatialization(map->sources[i_source], 1);
			audio_source_pos(map->sources[i_source],
				map->sprites[i_sprite]->pos);
		}
	}
	return (1);
}

static int	allocate_source_array(t_map *map)
{
	int	i;
	int	nbr_collec;

	if (!map->sprites || !map->tracks[SOUND_COLLEC])
		return (1);
	i = -1;
	nbr_collec = 0;
	while (map->sprites[++i])
		nbr_collec += map->sprites[i]->is_collectible;
	if (!nbr_collec)
		return (1);
	map->sources = calloc(nbr_collec + 1, sizeof(t_a_source));
	if (!map->sources)
		return (put_error(0, E_FAIL_MEM, 0, 0));
	return (1);
}
