#include "cimmerian.h"

int	clamp_max(int n, int max)
{
	if (n > max)
		return (max);
	return (n);
}

double	f_clamp_max(double n, double max)
{
	if (n > max)
		return (max);
	return (n);
}
