#include "olafur.h"

static t_sound_type	get_sound_type(t_map *map, int i);
static int			check_flag(const char *var, int *flag);
static int			load_track(t_man *man, t_map *map, int i,
						t_sound_type type);

int	process_sound_assets(t_man *man, t_map *map)
{
	int				i;
	int				flags[6];
	t_sound_type	type;

	bzero(flags, 6 * sizeof(int));
	i = 0;
	while (map->pars.vars[i])
	{
		type = get_sound_type(map, i);
		if (type)
		{
			if (!check_flag(map->pars.vars[i][0], flags + type)
				|| !load_track(man, map, i, type))
				return (0);
		}
		else
			++i;
	}
	return (1);
}

static t_sound_type	get_sound_type(t_map *map, int i)
{
	if (!strcmp(map->pars.vars[i][0], "S_MUSIC"))
		return (SOUND_MUSIC);
	else if (!strcmp(map->pars.vars[i][0], "S_AMBIENCE"))
		return (SOUND_AMBIENCE);
	else if (!strcmp(map->pars.vars[i][0], "S_WIN"))
		return (SOUND_SUCCESS);
	else if (!strcmp(map->pars.vars[i][0], "S_LOSE"))
		return (SOUND_FAILURE);
	else if (!strcmp(map->pars.vars[i][0], "S_COLLEC"))
		return (SOUND_COLLEC);
	return (0);
}

static int	check_flag(const char *var, int *flag)
{
	if (*flag)
		return (put_error(0, E_DUP_VAR, var, 0));
	*flag = 1;
	return (1);
}

static int	load_track(t_man *man, t_map *map, int i, t_sound_type type)
{
	if (!map->pars.vars[i][1])
		return (put_error(0, E_VAR_VAL, map->pars.vars[i][0], 0));
	else if (map->pars.vars[i][2])
		return (put_error(0, E_VAR_VALS, map->pars.vars[i][0], 0));
	if (strcmp(map->pars.vars[i][1], "null"))
	{
		map->tracks[type] = audio_track_create(&man->audio,
				map->pars.vars[i][1], type == SOUND_COLLEC);
		if (!map->tracks[type])
			return (put_error(0, E_NO_TRACK, map->pars.vars[i][1], 0));
	}
	remove_var_line(map, i);
	return (1);
}
