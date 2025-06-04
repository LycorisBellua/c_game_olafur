#include "cimmerian.h"

/*
int	find_text(char *line, int *size)
{
	int	i;

	i = 0;
	*size = 0;
	skip_char(line, &i, ' ');
	while (line[i + *size] && line[i + *size] != ' ')
		(*size)++;
	return (i + (*size));
}

int	isvalid(char c, char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (c == str[i])
			return (1);
		i++;
	}
	return (0);
}

int	onlyvalids(char *str, char *source)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if (!isvalid(str[i], source))
			return (0);
		i++;
	}
	return (1);
}

int	skip_char(char *str, int *i, char c)
{
	if (!str)
		return (-10000);
	while (str[*i] && str[*i] == c)
		(*i)++;
	return (*i);
}

char	*append(char **s1, char *s2)
{
	char	*tmp;

	tmp = strjoin(*s1, s2);
	free(*s1);
	free(s2);
	*s1 = tmp;
	return (*s1);
}

char	*ft_strncpy(char *dest, const char *src, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && src[i])
	{
		dest[i] = src[i];
		++i;
	}
	while (i < n)
		dest[i++] = 0;
	return (dest);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub;

	if (!s)
		return (0);
	sub = malloc((len + 1) * sizeof(char));
	if (!sub)
		return (0);
	if (start >= strlen(s))
		ft_strncpy(sub, "", len);
	else
		ft_strncpy(sub, s + start, len);
	sub[len] = 0;
	return (sub);
}

int	revncmp(const char *s1, const char *s2, size_t n)
{
	size_t	len1;
	size_t	len2;

	if (!n || (!s1 && !s2))
		return (0);
	else if (!s1)
		return (-1);
	else if (!s2)
		return (1);
	len1 = strlen(s1);
	len2 = strlen(s2);
	if (!n)
		return (0);
	while (n--)
	{
		if (!len1 || !len2 || s1[--len1] != s2[--len2])
			break ;
	}
	return ((unsigned char)s1[len1] - (unsigned char)s2[len2]);
}
*/
