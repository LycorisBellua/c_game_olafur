#include "cimmerian.h"

/*
double	move_decimal(double n, unsigned int decimal)
{
	if (!n || !decimal)
		return (n);
	return (move_decimal(n / 10, --decimal));
}

double	atod(char *line, int *i)
{
	double			n;
	int				tmp;
	unsigned int	decimal;

	tmp = 0;
	decimal = 0;
	n = (double)atoi(line);
	while (isdigit(line[tmp]))
		tmp++;
	if (line[tmp] == '.')
	{
		tmp++;
		if (!isdigit(line[tmp]))
		{
			*i = -1;
			return (0);
		}
		while (isdigit(line[tmp + decimal]))
			decimal++;
		n += move_decimal((double)atoi(line + tmp), decimal);
		tmp += decimal;
	}
	*i += tmp;
	return (n);
}
*/
