#include "cimmerian.h"

void	free_arr(void **arr, void (free_fct)(void *))
{
	size_t	i;

	if (!arr)
		return ;
	if (free_fct)
	{
		i = 0;
		while (arr[i])
		{
			free_fct(arr[i]);
			++i;
		}
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

size_t	count_arr_elems(void **arr)
{
	size_t	count;

	count = 0;
	if (!arr)
		return (count);
	while (arr[count])
		++count;
	return (count);
}
