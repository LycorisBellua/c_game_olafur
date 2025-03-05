#include "cimmerian.h"

int	clamp_min(int n, int min)
{
	if (n < min)
		return (min);
	return (n);
}

double	clamp_min_f(double n, double min)
{
	if (n < min)
		return (min);
	return (n);
}
