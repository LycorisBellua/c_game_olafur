#include "cimmerian.h"

int	get_rgb_num(const char *arg)
{
	int	i;
	int	num;

	i = 0;
	while (arg[i])
	{
		if (!isdigit(arg[i]))
			return (-1);
		++i;
	}
	if (!i || i > 3)
		return (-1);
	num = atoi(arg);
	if (num > 255)
		return (-1);
	return (num);
}

int	get_num(const char *arg)
{
	int	i;

	i = 0;
	while (arg[i])
	{
		if (!isdigit(arg[i]))
			return (-1);
		++i;
	}
	if (!i || i > 10)
		return (-1);
	if (i == 10)
	{
		i = 0;
		while (arg[i])
		{
			if (arg[i] < "2147483647"[i])
				return (atoi(arg));
			else if (arg[i] > "2147483647"[i])
				return (-1);
			++i;
		}
	}
	return (atoi(arg));
}
