#include "olafur.h"

int	put_error(t_man *man, const char *msg, const char *arg, int errnum)
{
	if (man)
		deinit(man);
	if (msg)
	{
		fprintf(stderr, "Error: %s", msg);
		if (arg)
			fprintf(stderr, ": %s", arg);
		fprintf(stderr, "\n");
	}
	return (errnum);
}
