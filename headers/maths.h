#ifndef __MATHS_H__
#define __MATHS_H__

#define ABS(x)             ((x) < 0 ? -(x) : (x))
#define NORMALIZE(x)       ((x) < 0 ? -1 : (x) > 0 ? 1 : 0)
#define MIN(a, b)          ((a) < (b) ? (a) : (b))
#define MAX(a, b)          ((a) > (b) ? (a) : (b))
#define CLAMP(x, min, max) ((x) < (min) ? (min) : (x) > (max) ? (max) : (x))
#define CLAMP_MIN(x, min)  ((x) < (min) ? (min) : (x))
#define CLAMP_MAX(x, max)  ((x) > (max) ? (max) : (x))

void swap(int* a, int* b);

#endif

