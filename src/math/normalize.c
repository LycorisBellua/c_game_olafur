#include "cimmerian.h"

int	norm(int n)
{
	if (n < 0)
		return (-1);
	else if (n > 0)
		return (1);
	return (0);
}

double	f_norm(double n)
{
	if (n < 0)
		return (-1);
	else if (n > 0)
		return (1);
	return (0);
}
