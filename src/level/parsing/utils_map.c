#include "cimmerian.h"

static void	biggest_digit(char c, int *n);

void	find_longest_line(char *s, t_ivec2 *size, int *n_texture)
{
	int	longest;
	int	count_line;
	int	start;
	int	end;

	end = 0;
	start = end;
	longest = end;
	count_line = end + 1;
	if (!s)
		return ;
	while (s[end])
	{
		biggest_digit(s[end], n_texture);
		if ((s[end] == '\n') && ++count_line)
		{
			if (end - start > longest)
				longest = end - start;
			start = end + 1;
		}
		end++;
	}
	if (s[end - 1] == '\n')
		--count_line;
	set_ivec2(size, longest, count_line);
}

int	find_endline(char *lines)
{
	int	i;

	i = 0;
	while (lines[i])
	{
		if (lines[i] == '\n')
			return (i);
		(i)++;
	}
	return (i);
}

static void	biggest_digit(char c, int *n)
{
	int	tmp;

	if (isdigit(c))
	{
		tmp = c - '0';
		if (*n < tmp)
			*n = tmp;
	}
}
