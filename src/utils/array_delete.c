#include "cimmerian.h"

void	*extract_arr_elem(void **array, size_t index)
{
	void	*to_extract;

	if (!array || index >= count_arr_elems(array))
		return (0);
	to_extract = array[index];
	while (array[index])
	{
		array[index] = array[index + 1];
		++index;
	}
	return (to_extract);
}

void	**extract_arr_elems(void **array, size_t from, size_t to)
{
	size_t	i;
	size_t	len;
	void	**to_extract;

	if (!array || from > to)
		return (0);
	len = count_arr_elems(array);
	if (!len)
		return (0);
	if (to >= len)
		to = len - 1;
	to_extract = calloc(to - from + 2, sizeof(void *));
	if (!to_extract)
		return (0);
	i = 0;
	while ((from + i) <= to)
	{
		to_extract[i] = array[from + i];
		++i;
	}
	remove_arr_elems(array, from, to, 0);
	return (to_extract);
}

void	remove_arr_elems(void **array, size_t from, size_t to,
	void (*free_element)(void *))
{
	size_t	i;

	if (!array || from > to)
		return ;
	i = from;
	while (array[i] && i <= to)
	{
		if (free_element)
			free_element(array[i]);
		++i;
	}
	while (array[i])
	{
		array[from] = array[i];
		++from;
		++i;
	}
	array[from] = 0;
	return ;
}
