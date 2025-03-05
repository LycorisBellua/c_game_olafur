#include "cimmerian.h"

double	floor_f(double n)
{
	return ((long)n);
}

double	ceil_f(double n)
{
	return ((long)n + 1);
}

double	round_f(double n)
{
	if (n - (long)n < 0.5)
		return (floor_f(n));
	return (ceil_f(n));
}

/* Check up to 7 digits after the floating point */
int	double_equality(double a, double b)
{
	if ((a < 0 && b > 0) || (a > 0 && b < 0) || (long)a != (long)b)
		return (0);
	a -= (long)a;
	b -= (long)b;
	a *= pow_f(10, 7);
	b *= pow_f(10, 7);
	return ((long)a == (long)b);
}
