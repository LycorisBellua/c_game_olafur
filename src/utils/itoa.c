#include "cimmerian.h"

static int	get_num_len(long n);

char	*itoa(int number)
{
	long	n;
	int		num_len;
	char	*str;

	n = number;
	num_len = get_num_len(n);
	str = malloc((num_len + 1) * sizeof(char));
	if (!str)
		return (0);
	str[num_len--] = 0;
	str[num_len] = '0';
	if (n < 0)
	{
		str[0] = '-';
		str[num_len--] = '0' - n % 10;
		n /= 10;
		n = -n;
	}
	while (n > 0)
	{
		str[num_len--] = '0' + n % 10;
		n /= 10;
	}
	return (str);
}

static int	get_num_len(long n)
{
	int	num_len;

	num_len = 0;
	if (!n)
		return (1);
	else if (n < 0)
	{
		if (n == -2147483648)
			return (11);
		++num_len;
		n = -n;
	}
	while (n > 0)
	{
		++num_len;
		n /= 10;
	}
	return (num_len);
}
