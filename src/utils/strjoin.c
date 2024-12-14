#include "cimmerian.h"

char	*strjoin(char const *s1, char const *s2)
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
