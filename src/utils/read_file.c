#include "olafur.h"

char	*read_file(const char *filepath)
{
	int		fd;
	char	*tmp1;
	char	*tmp2;
	char	*tmp3;

	fd = open(filepath, O_RDONLY);
	if (fd < 0)
	{
		printf("Error: Couldn't open file \"%s\" for reading\n", filepath);
		return (0);
	}
	tmp1 = gnl(fd);
	tmp2 = gnl(fd);
	while (tmp2)
	{
		tmp3 = strjoin(tmp1, tmp2);
		free(tmp1);
		free(tmp2);
		tmp1 = tmp3;
		tmp2 = gnl(fd);
	}
	close(fd);
	return (tmp1);
}
