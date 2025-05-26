#include "cimmerian.h"

int	put_error(t_man *man, const char *msg, const char *arg, int errno)
{
	if (man)
		deinit(man);
	if (msg)
	{
		dprintf(2, "Error: %s", msg);
		if (arg)
			dprintf(2, ": %s", arg);
		dprintf(2, "\n");
	}
	return (errno);
}
