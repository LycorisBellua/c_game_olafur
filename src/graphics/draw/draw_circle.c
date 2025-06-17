#include "olafur.h"

static void		update_p_and_decision(int in_loop, t_ivec2 *p, int *decision,
					int radius);
static t_color	interpolate_color(t_color start, t_color end, double ratio);

/* Bresenham/Midpoint Circle Drawing algorithm */
void	draw_circle(t_man *man, t_vert center, int radius)
{
	t_ivec2	p;
	int		decision;
	t_ivec2	point;

	update_p_and_decision(0, &p, &decision, radius);
	while (p.y <= p.x)
	{
		set_ivec2(&point, center.coord.x - p.x, center.coord.y - p.y);
		draw_point(man, center.color, point.x, point.y);
		draw_point(man, center.color, center.coord.x + p.x, point.y);
		set_ivec2(&point, center.coord.x - p.x, center.coord.y + p.y);
		draw_point(man, center.color, point.x, point.y);
		draw_point(man, center.color, center.coord.x + p.x, point.y);
		set_ivec2(&point, center.coord.x - p.y, center.coord.y - p.x);
		draw_point(man, center.color, point.x, point.y);
		draw_point(man, center.color, center.coord.x + p.y, point.y);
		set_ivec2(&point, center.coord.x - p.y, center.coord.y + p.x);
		draw_point(man, center.color, point.x, point.y);
		draw_point(man, center.color, center.coord.x + p.y, point.y);
		update_p_and_decision(1, &p, &decision, radius);
	}
	return ;
}

/* Bresenham/Midpoint Circle Drawing algorithm */
void	draw_circle_full(t_man *man, t_vert center, int radius)
{
	t_ivec2	p;
	int		decision;
	t_vert	v1;
	t_vert	v2;

	update_p_and_decision(0, &p, &decision, radius);
	v1.color = center.color;
	v2.color = v1.color;
	while (p.y <= p.x)
	{
		set_ivec2(&v1.coord, center.coord.x - p.x, center.coord.y - p.y);
		set_ivec2(&v2.coord, center.coord.x + p.x, v1.coord.y);
		draw_line(man, v1, v2);
		v1.coord.y = center.coord.y + p.y;
		v2.coord.y = v1.coord.y;
		draw_line(man, v1, v2);
		set_ivec2(&v1.coord, center.coord.x - p.y, center.coord.y - p.x);
		set_ivec2(&v2.coord, center.coord.x + p.y, v1.coord.y);
		draw_line(man, v1, v2);
		v1.coord.y = center.coord.y + p.x;
		v2.coord.y = v1.coord.y;
		draw_line(man, v1, v2);
		update_p_and_decision(1, &p, &decision, radius);
	}
	return ;
}

void	draw_circle_full_gradient(t_man *man, t_vert center, int radius,
	t_color edge)
{
	t_ivec2	p;
	int		distance_squared;
	double	linear_ratio;
	double	adjusted_ratio;
	t_color	c;

	p.y = -radius;
	while (p.y <= radius)
	{
		p.x = -radius;
		while (p.x <= radius)
		{
			distance_squared = p.x * p.x + p.y * p.y;
			if (distance_squared <= radius * radius)
			{
				linear_ratio = sqrt_approx((double)distance_squared) / radius;
				adjusted_ratio = linear_ratio * linear_ratio;
				c = interpolate_color(center.color, edge, adjusted_ratio);
				draw_point(man, c, center.coord.x + p.x, center.coord.y + p.y);
			}
			++p.x;
		}
		++p.y;
	}
	return ;
}

static void	update_p_and_decision(int in_loop, t_ivec2 *p, int *decision,
	int radius)
{
	if (!in_loop)
	{
		set_ivec2(p, radius, 0);
		*decision = 1 - radius;
	}
	else if (*decision <= 0)
		*decision += 2 * ++p->y + 1;
	else
		*decision += 2 * (++p->y - --p->x) + 1;
	return ;
}

static t_color	interpolate_color(t_color start, t_color end, double ratio)
{
	t_color	result;

	result.r = (1 - ratio) * start.r + ratio * end.r;
	result.g = (1 - ratio) * start.g + ratio * end.g;
	result.b = (1 - ratio) * start.b + ratio * end.b;
	result.a = (1 - ratio) * start.a + ratio * end.a;
	return (result);
}
