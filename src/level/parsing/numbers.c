#include "olafur.h"

static int		is_valid_flt(const char *arg, int *i_point);
static float	get_flt_point(const char *arg, int i_point);

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
	if (!i || i > 5 || (arg[0] == '0' && i > 1))
		return (-1);
	if (i == 5)
	{
		i = 0;
		while (arg[i])
		{
			if (arg[i] < "15000"[i])
				return (atoi(arg));
			else if (arg[i] > "15000"[i])
				return (-1);
			++i;
		}
	}
	return (atoi(arg));
}

float	get_num_flt(const char *arg)
{
	int		i_point;
	int		nbr_whole;
	char	*substr;
	float	flt_point;

	if (!is_valid_flt(arg, &i_point))
		return ((float)put_error(0, E_BAD_COORD_F, arg, -1));
	else if (i_point == 0)
		nbr_whole = 0;
	else if (i_point < 0)
		nbr_whole = get_num_int(arg);
	else
	{
		substr = get_substr(arg, 0, i_point);
		if (!substr)
			return ((float)put_error(0, E_FAIL_MEM, 0, -1));
		nbr_whole = get_num_int(substr);
		free(substr);
		if (nbr_whole < 0)
			return ((float)put_error(0, E_BAD_COORD_F, arg, -1));
	}
	flt_point = get_flt_point(arg, i_point);
	if (flt_point < 0)
		return ((float)put_error(0, E_FLT_EXTRA_DIGIT, arg, -1));
	return (nbr_whole + flt_point);
}

static int	is_valid_flt(const char *arg, int *i_point)
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

static float	get_flt_point(const char *arg, int i_point)
{
	int	nbr_point;

	nbr_point = 0;
	if (i_point >= 0)
	{
		if (arg[i_point + 1])
		{
			nbr_point = (arg[i_point + 1] - '0') * 10;
			if (arg[i_point + 2])
			{
				nbr_point += arg[i_point + 2] - '0';
				if (arg[i_point + 3])
					return (-1);
			}
		}
	}
	return (nbr_point / 100.0);
}
