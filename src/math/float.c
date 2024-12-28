#include "cimmerian.h"

double	f_floor(double n)
{
	return ((int)n);
}

double	f_ceil(double n)
{
	return ((int)n + 1);
}

double	f_round(double n)
{
	if (n - (int)n < 0.5)
		return (f_floor(n));
	return (f_ceil(n));
}

/* Check up to 7 digits after the floating point */
int	double_equality(double a, double b)
{
	if ((a < 0 && b > 0) || (a > 0 && b < 0) || (int)a != (int)b)
		return (0);
	a -= (int)a;
	b -= (int)b;
	a *= f_pow(10, 7);
	b *= f_pow(10, 7);
	return ((int)a == (int)b);
}
