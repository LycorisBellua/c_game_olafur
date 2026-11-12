#include "olafur.h"

int	put_error(const char *msg, const char *arg, int errnum)
{
	if (msg)
	{
		fprintf(stderr, "Error: %s", msg);
		if (arg)
			fprintf(stderr, ": %s", arg);
		fprintf(stderr, "\n");
	}
	return (errnum);
}
