#include "cimmerian.h"

int	clamp_max(int n, int max)
{
	if (n > max)
		return (max);
	return (n);
}

double	clamp_max_f(double n, double max)
{
	if (n > max)
		return (max);
	return (n);
}
