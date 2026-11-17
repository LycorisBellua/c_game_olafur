#include "olafur.h"

/* The list grows by doubling its capacity, starting at 8. */
int	push_strlist(t_strlist *list, const char *s)
{
	char	**new_items;
	size_t	new_cap;

	if (list->count == list->cap)
	{
		new_cap = 8;
		if (list->cap)
			new_cap = list->cap * 2;
		new_items = realloc(list->items, new_cap * sizeof(char *));
		if (!new_items)
			return (put_error(E_FAIL_MEM, 0, 0));
		list->items = new_items;
		list->cap = new_cap;
	}
	list->items[list->count] = strdup(s);
	if (!list->items[list->count])
		return (put_error(E_FAIL_MEM, 0, 0));
	++list->count;
	return (1);
}

void	sort_strlist(t_strlist *list)
{
	size_t	i;
	size_t	j;
	char	*key;

	i = 1;
	while (i < list->count)
	{
		key = list->items[i];
		j = i;
		while (j > 0 && strcmp(list->items[j - 1], key) > 0)
		{
			list->items[j] = list->items[j - 1];
			--j;
		}
		list->items[j] = key;
		++i;
	}
	return ;
}

void	free_strlist(t_strlist *list)
{
	size_t	i;

	i = 0;
	while (i < list->count)
		free(list->items[i++]);
	free(list->items);
	memset(list, 0, sizeof(t_strlist));
	return ;
}
