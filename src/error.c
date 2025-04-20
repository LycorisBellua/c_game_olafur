#include "cimmerian.h"

int	put_error(t_man *man, char *msg, int errno)
{
	if (man)
		deinit(man);
	if (msg)
		dprintf(2, "Error: %s\n", msg);
	return (errno);
}
