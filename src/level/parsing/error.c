#include "cimmerian.h"

int	put_error_wall(char *tex[10][4], const char *msg, const char *arg)
{
	int	i;
	int	j;

	put_error(0, msg, arg, 0);
	i = 0;
	while (i < 10)
	{
		j = 0;
		while (j < 4)
		{
			free(tex[i][j]);
			tex[i][j] = 0;
			++j;
		}
		++i;
	}
	return (0);
}
