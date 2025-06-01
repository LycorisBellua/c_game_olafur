#include "cimmerian.h"

void	put_error_and_exit(t_man *man, char *msg, int shouldexit, int err)
{
	if (man)
		deinit(man);
	if (msg)
		dprintf(2, "Error: %s\n", msg);
	if (shouldexit)
		exit(err);
}
