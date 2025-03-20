#include "cimmerian.h"

char	*strjoin(const char *s1, const char *s2)
{
	size_t	len1;
	size_t	len2;
	char	*joined;

	if (!s1 || !s2)
		return (0);
	len1 = strlen(s1);
	len2 = strlen(s2);
	joined = malloc((len1 + len2 + 1) * sizeof(char));
	if (!joined)
		return (0);
	strcpy(joined, s1);
	strcpy(joined + len1, s2);
	return (joined);
}

char	*strjoin_arr(char **arr, size_t start, size_t end)
{
	char	*tmp1;
	char	*tmp2;

	if (!arr || start > end)
		return (0);
	tmp1 = malloc(1 * sizeof(char));
	if (!tmp1)
		return (0);
	tmp1[0] = 0;
	while (start < end && arr[start])
	{
		tmp2 = strjoin(tmp1, arr[start]);
		free(tmp1);
		tmp1 = tmp2;
		++start;
	}
	return (tmp1);
}
