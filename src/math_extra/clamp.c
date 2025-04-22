#include "cimmerian.h"

int	clamp(int n, int min, int max)
{
	if (n < min)
		return (min);
	else if (n > max)
		return (max);
	return (n);
}

double	fclamp(double n, double min, double max)
{
	if (n < min)
		return (min);
	else if (n > max)
		return (max);
	return (n);
}

int	norm(int n)
{
	if (n < 0)
		return (-1);
	else if (n > 0)
		return (1);
	return (0);
}
