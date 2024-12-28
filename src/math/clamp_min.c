#include "cimmerian.h"

int	clamp_min(int n, int min)
{
	if (n < min)
		return (min);
	return (n);
}

double	f_clamp_min(double n, double min)
{
	if (n < min)
		return (min);
	return (n);
}
