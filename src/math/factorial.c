#include "cimmerian.h"

double	factorial(int n)
{
	if (n < 2)
		return (1.0);
	return (n * factorial(n - 1));
}
