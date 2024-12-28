#include "cimmerian.h"

/* 32-bit Xorshift pseudo-RNG */
int	rng_minmax(int *seed, int min, int max)
{
	unsigned int	random;

	if (!*seed)
		*seed = 1;
	random = *seed;
	random ^= random << 13;
	random ^= random >> 17;
	random ^= random << 5;
	*seed = random;
	return (random % (max - min + 1) + min);
}
