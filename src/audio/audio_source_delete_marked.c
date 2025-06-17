#include "olafur.h"

static int	is_source_to_be_deleted(t_a_source *s);

void	audio_source_delete_marked(t_map *map)
{
	int	i;

	if (!map || !map->sources)
		return ;
	i = 0;
	while (map->sources[i])
	{
		if (is_source_to_be_deleted(map->sources[i]))
			remove_arr_elems((void **)map->sources, i, i, audio_source_delete);
		else
			++i;
	}
	return ;
}

static int	is_source_to_be_deleted(t_a_source *s)
{
	ALint	state;

	if (!s || !s->to_be_deleted)
		return (0);
	alGetSourcei(s->source, AL_SOURCE_STATE, &state);
	return (state != AL_PLAYING);
}
