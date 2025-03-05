#include "cimmerian.h"

int	clamp(int n, int min, int max)
{
	if (n < min)
		return (min);
	else if (n > max)
		return (max);
	return (n);
}

double	clamp_f(double n, double min, double max)
{
	if (n < min)
		return (min);
	else if (n > max)
		return (max);
	return (n);
}

double	clamp_rad(double rad)
{
	if (rad < 0)
		return (rad + RAD_360);
	else if (rad > RAD_360)
		return (rad - RAD_360);
	return (rad);
}
