#include "cimmerian.h"

void	swap_elements(void **a, void **b)
{
	void	*tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
	return ;
}
