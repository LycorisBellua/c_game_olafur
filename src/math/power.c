#include "cimmerian.h"

double	pow_f(double base, int exponent)
{
	int		i;
	double	result;

	i = 0;
	result = 1.0;
	while (i < exponent)
	{
		result *= base;
		++i;
	}
	return (result);
}
