#include "cimmerian.h"

double	move_decimal(double n, unsigned int decimal)
{
	if (n == 0 || decimal == 0)
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
	while (isdigit(line[tmp]) == 1)
		tmp++;
	if (line[tmp] == '.')
	{
		tmp++;
		if (isdigit(line[tmp]) == 0)
		{
			*i = -1;
			return (0);
		}
		while (isdigit(line[tmp + decimal]) == 1)
			decimal++;
		n += move_decimal((double)atoi(line + tmp), decimal);
		tmp += decimal;
	}
	*i += tmp;
	return (n);
}
