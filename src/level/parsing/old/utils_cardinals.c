#include "cimmerian.h"

/*
int	check_doublewall(char *line, t_tex_type *type)
{
	if (!strncmp(line, "NO", 2) && type->tex_north
		&& type->tex_north->id != NULL)
		return (-1);
	else if (!strncmp(line, "SO", 2) && type->tex_south
		&& type->tex_south->id != NULL)
		return (-1);
	else if (!strncmp(line, "EA", 2) && type->tex_east
		&& type->tex_east->id != NULL)
		return (-1);
	else if (!strncmp(line, "WE", 2) && type->tex_west
		&& type->tex_west->id != NULL)
		return (-1);
	return (0);
}

int	check_texfilled(t_tex_type *types, int *n_texture)
{
	int	n;
	int	no_id;

	n = 1;
	no_id = 0;
	while (n <= *n_texture && !no_id)
	{
		if (!types[n].tex_north || !types[n].tex_north->id)
			no_id--;
		if (!types[n].tex_south || !types[n].tex_south->id)
			no_id--;
		if (!types[n].tex_west || !types[n].tex_west->id)
			no_id--;
		if (!types[n].tex_east || !types[n].tex_east->id)
			no_id--;
		n++;
	}
	if (no_id < 0)
	{
		*n_texture = -1;
		return (0);
	}
	return (1);
}
*/
