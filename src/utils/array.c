#include "cimmerian.h"

void	free_arr(void **arr, void (free_fct)(void *))
{
	size_t	i;

	if (!arr || !free_fct)
		return ;
	i = 0;
	while (arr[i])
	{
		free_fct(arr[i]);
		++i;
	}
	free(arr);
	return ;
}

void	swap_elements(void **a, void **b)
{
	void	*tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
	return ;
}
