#include "olafur.h"

static int		is_valid_dbl(const char *arg, int *i_point);
static double	get_dbl_point(const char *arg, int i_point);

int	get_num_rgb(const char *arg)
{
	int	i;
	int	num;

	i = 0;
	while (arg[i])
	{
		if (!isdigit(arg[i]))
			return (-1);
		++i;
	}
	if (!i || i > 3 || (arg[0] == '0' && i > 1))
		return (-1);
	num = atoi(arg);
	if (num > 255)
		return (-1);
	return (num);
}

int	get_num_int(const char *arg)
{
	int	i;

	i = 0;
	while (arg[i])
	{
		if (!isdigit(arg[i]))
			return (-1);
		++i;
	}
	if (!i || i > 10 || (arg[0] == '0' && i > 1))
		return (-1);
	if (i == 10)
	{
		i = 0;
		while (arg[i])
		{
			if (arg[i] < "2147483647"[i])
				return (atoi(arg));
			else if (arg[i] > "2147483647"[i])
				return (-1);
			++i;
		}
	}
	return (atoi(arg));
}

double	get_num_dbl(const char *arg)
{
	int		i_point;
	int		nbr_whole;
	char	*substr;

	if (!is_valid_dbl(arg, &i_point))
		return (-1);
	else if (i_point == 0)
		nbr_whole = 0;
	else if (i_point < 0)
		nbr_whole = get_num_int(arg);
	else
	{
		substr = get_substr(arg, 0, i_point);
		if (!substr)
			return ((double)put_error(0, E_FAIL_MEM, 0, -1));
		nbr_whole = get_num_int(substr);
		free(substr);
		if (nbr_whole < 0)
			return (-1);
	}
	return (nbr_whole + get_dbl_point(arg, i_point));
}

static int	is_valid_dbl(const char *arg, int *i_point)
{
	int	i;

	i = 0;
	*i_point = -1;
	while (arg[i])
	{
		if (arg[i] == '.')
		{
			if (*i_point >= 0)
				return (0);
			*i_point = i;
		}
		else if (!isdigit(arg[i]))
			return (0);
		++i;
	}
	if (i == 1 && *i_point == 0)
		return (0);
	return (1);
}

static double	get_dbl_point(const char *arg, int i_point)
{
	int	nbr_point;

	nbr_point = 0;
	if (i_point >= 0)
	{
		if (arg[i_point + 1])
		{
			nbr_point = (arg[i_point + 1] - '0') * 10;
			if (arg[i_point + 2])
				nbr_point += arg[i_point + 2] - '0';
		}
	}
	return (nbr_point / 100.0);
}
