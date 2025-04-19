#include "cimmerian.h"

t_vec2	get_dir_from_cardinal(char c)
{
	t_vec2	dir;

	dir.x = 0;
	dir.y = 0;
	if (c == 'N')
		dir.y = -1;
	else if (c == 'S')
		dir.y = 1;
	else if (c == 'W')
		dir.x = -1;
	else if (c == 'E')
		dir.x = 1;
	return (dir);
}

char	opp_cardinal(char cardinal)
{
	if (cardinal == 'N')
		return ('S');
	else if (cardinal == 'S')
		return ('N');
	else if (cardinal == 'E')
		return ('W');
	else if (cardinal == 'W')
		return ('E');
	return (0);
}
