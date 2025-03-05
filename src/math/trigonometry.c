#include "cimmerian.h"

static double	is_close_to_zero(double n);

double	cos_f(double rad)
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
			result += pow_f(-1, n) * pow_f(rad, 2 * n) / fact;
		++n;
	}
	return (result);
}

double	sin_f(double rad)
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
			result += pow_f(-1, n) * pow_f(rad, 2 * n + 1) / fact;
		++n;
	}
	return (result);
}

double	tan_f(double rad)
{
	double	cos;

	cos = cos_f(rad);
	if (is_close_to_zero(cos))
		return (0.0);
	return (sin_f(rad) / cos);
}

static double	is_close_to_zero(double n)
{
	return (abs_f(n) < 1e-15);
}
