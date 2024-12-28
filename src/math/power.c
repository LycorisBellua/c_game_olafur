#include "cimmerian.h"

double	f_pow(double base, int exponent)
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
