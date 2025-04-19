#include "cimmerian.h"

#define COLINEAR 0
#define CLOCKWISE 1
#define COUNTER_CLOCKWISE 2

static int	orientation(t_vec2 start, t_vec2 point, t_vec2 end);
static int	is_on_segment(t_vec2 start, t_vec2 point, t_vec2 end);

void	set_vec2(t_vec2 *v, double x, double y)
{
	v->x = x;
	v->y = y;
	return ;
}

void	set_ivec2(t_ivec2 *v, int x, int y)
{
	v->x = x;
	v->y = y;
	return ;
}

int	do_segments_intersect(t_vec2 start1, t_vec2 end1, t_vec2 start2,
	t_vec2 end2)
{
	int	o1;
	int	o2;
	int	o3;
	int	o4;

	o1 = orientation(start1, end1, start2);
	o2 = orientation(start1, end1, end2);
	o3 = orientation(start2, end2, start1);
	o4 = orientation(start2, end2, end1);
	return ((o1 != o2 && o3 != o4)
		|| (o1 == 0 && is_on_segment(start1, start2, end1))
		|| (o2 == 0 && is_on_segment(start1, end2, end1))
		|| (o3 == 0 && is_on_segment(start2, start1, end2))
		|| (o4 == 0 && is_on_segment(start2, end1, end2)));
}

static int	orientation(t_vec2 start, t_vec2 point, t_vec2 end)
{
	double	val;

	val = (point.y - start.y) * (end.x - point.x)
		- (point.x - start.x) * (end.y - point.y);
	if (!val)
		return (COLINEAR);
	else if (val > 0)
		return (CLOCKWISE);
	return (COUNTER_CLOCKWISE);
}

static int	is_on_segment(t_vec2 start, t_vec2 point, t_vec2 end)
{
	return (point.x <= fmax(start.x, end.x)
		&& point.x >= fmin(start.x, end.x)
		&& point.y <= fmax(start.y, end.y)
		&& point.y >= fmin(start.y, end.y));
}
