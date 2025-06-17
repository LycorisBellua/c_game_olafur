#include "olafur.h"

static int	load_json_index(t_man *man, t_map *map, int i);

int	process_img_assets(t_man *man, t_map *map)
{
	int	i;

	i = 0;
	while (map->pars.vars[i])
	{
		if (!strcmp(map->pars.vars[i][0], "IMG"))
		{
			if (!map->pars.vars[i][1])
				return (put_error(0, E_VAR_VAL, map->pars.vars[i][0], 0));
			else if (map->pars.vars[i][2])
				return (put_error(0, E_VAR_VALS, map->pars.vars[i][0], 0));
			else if (strcmp(map->pars.vars[i][1], "null")
				&& !load_json_index(man, map, i))
				return (0);
			remove_var_line(map, i);
		}
		else
			++i;
	}
	return (1);
}

static int	load_json_index(t_man *man, t_map *map, int i)
{
	const char	*name;
	size_t		len;

	name = get_filename(map->pars.vars[i][1]);
	len = strlen(name);
	if (len < 6 || strcmp(name + len - 5, ".json"))
		return (put_error(0, E_BAD_JSON, map->pars.vars[i][1], 0));
	else if (!update_image_array(man, map->pars.vars[i][1]))
		return (put_error(0, E_FAIL_JSON, map->pars.vars[i][1], 0));
	return (1);
}
