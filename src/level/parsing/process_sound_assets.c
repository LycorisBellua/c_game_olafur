#include "cimmerian.h"

static int	load_music_track(t_man *man, t_map *map, int i);

int	process_sound_assets(t_man *man, t_map *map)
{
	int	i;

	i = 0;
	while (map->pars.vars[i])
	{
		if (!strcmp(map->pars.vars[i][0], "M_BG"))
		{
			if (!map->pars.vars[i][1])
				return (put_error(0, E_VAR_VAL, map->pars.vars[i][0], 0));
			else if (map->pars.vars[i][2])
				return (put_error(0, E_VAR_VALS, map->pars.vars[i][0], 0));
			else if (strcmp(map->pars.vars[i][1], "null")
				&& !load_music_track(man, map, i))
				return (0);
			remove_var_line(map, i);
		}
		else
			++i;
	}
	return (1);
}

static int	load_music_track(t_man *man, t_map *map, int i)
{
	if (map->music_track)
		return (put_error(0, E_DUP_VAR, map->pars.vars[i][0], 0));
	map->music_track = audio_track_create(&man->audio, map->pars.vars[i][1]);
	if (!map->music_track)
		return (put_error(0, E_NO_TRACK, map->pars.vars[i][1], 0));
	return (1);
}
