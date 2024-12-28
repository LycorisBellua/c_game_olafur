#include "cimmerian.h"

static double	is_close_to_zero(double n);

double	f_cos(double rad)
{
	int		n;
	double	result;
	double	fact;

	n = 0;
	result = 0.0;
	while (n < 10)
	{
		fact = factorial(2 * n);
		if (!is_close_to_zero(fact))
			result += f_pow(-1, n) * f_pow(rad, 2 * n) / fact;
		++n;
	}
	return (result);
}

double	f_sin(double rad)
{
	int		n;
	double	result;
	double	fact;

	n = 0;
	result = 0.0;
	while (n < 10)
	{
		fact = factorial(2 * n + 1);
		if (!is_close_to_zero(fact))
			result += f_pow(-1, n) * f_pow(rad, 2 * n + 1) / fact;
		++n;
	}
	return (result);
}

double	f_tan(double rad)
{
	double	cos;

	cos = f_cos(rad);
	if (is_close_to_zero(cos))
		return (0.0);
	return (f_sin(rad) / cos);
}

static double	is_close_to_zero(double n)
{
	return (f_abs(n) < 1e-15);
}
