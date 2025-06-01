#include "cimmerian.h"

static size_t	count_split_nmemb(const char *s, char c);
static char		**populate_split_list(char **list, const char *s, char c);
static char		*get_substr(const char *s, unsigned int start, size_t len);

char	**split(const char *s, char c)
{
	char	**list;
	size_t	nmemb;

	if (!s)
		return (0);
	nmemb = count_split_nmemb(s, c);
	list = malloc((nmemb + 1) * sizeof(char *));
	if (!list)
		return (0);
	return (populate_split_list(list, s, c));
}

static size_t	count_split_nmemb(const char *s, char c)
{
	size_t	i;
	size_t	len;
	size_t	nmemb;

	if (!s)
		return (0);
	i = 0;
	len = 0;
	nmemb = 0;
	while (s[i])
	{
		if (s[i] != c)
			++len;
		else if (len)
		{
			++nmemb;
			len = 0;
		}
		++i;
	}
	if (len)
		++nmemb;
	return (nmemb);
}

static char	**populate_split_list(char **list, const char *s, char c)
{
	size_t	i;
	size_t	len;
	size_t	nmemb;

	i = 0;
	len = 0;
	nmemb = 0;
	if (s)
	{
		while (s[i])
		{
			if (s[i] != c)
				++len;
			else if (len)
			{
				list[nmemb++] = get_substr(s, i - len, len);
				len = 0;
			}
			++i;
		}
		if (len)
			list[nmemb++] = get_substr(s, i - len, len);
	}
	list[nmemb] = 0;
	return (list);
}

static char	*get_substr(const char *s, unsigned int start, size_t len)
{
	char	*sub;

	if (!s)
		return (0);
	sub = calloc(len + 1, sizeof(char));
	if (!sub)
		return (0);
	if (start < strlen(s))
		ft_strncpy(sub, s + start, len);
	return (sub);
}
