#include "cimmerian.h"

static void	draw_shape_full_unicolor(t_man *man, t_vert arr[], int len);
static void	draw_full_triangle(t_man *man, t_vert *v);
static void	draw_blended_p(t_man *man, t_vert *v, t_ivec2 p, float inv_denom);

void	draw_shape(t_man *man, t_vert arr[], int len)
{
	int	i;

	i = 0;
	while (i < len)
	{
		draw_line(man, arr[i], arr[(i + 1) % len]);
		++i;
	}
	return ;
}

/* TODO: Use Delaunay triangulation for shapes of more than 3 vertices */
void	draw_shape_full(t_man *man, t_vert arr[], int len)
{
	if (len < 3)
		draw_shape(man, arr, len);
	else if (len > 3)
		draw_shape_full_unicolor(man, arr, len);
	else
		draw_full_triangle(man, arr);
	return ;
}

/* Scanline Fill algorithm */
static void	draw_shape_full_unicolor(t_man *man, t_vert arr[], int len)
{
	int		i;
	int		j;
	int		y;
	int		x1;
	int		x2;
	int		x_intersection;
	int		ymin;
	int		ymax;
	t_vert	v1;
	t_vert	v2;

	v1.color = arr[0].color;
	v2.color = v1.color;
	ymin = arr[0].coord.y;
	ymax = arr[0].coord.y;
	i = 1;
	while (i < len)
	{
		if (arr[i].coord.y < ymin)
			ymin = arr[i].coord.y;
		if (arr[i].coord.y > ymax)
			ymax = arr[i].coord.y;
		++i;
	}
	y = ymin;
	while (y <= ymax)
	{
		x1 = man->frame.size.x - 1;
		x2 = 0;
		i = 0;
		while (i < len)
		{
			j = (i + 1) % len;
			if ((arr[i].coord.y <= y && arr[j].coord.y > y)
				|| (arr[j].coord.y <= y && arr[i].coord.y > y))
			{
				x_intersection = (arr[i].coord.x * (arr[j].coord.y - y)
						+ arr[j].coord.x * (y - arr[i].coord.y))
					/ (arr[j].coord.y - arr[i].coord.y);
				if (x_intersection < x1)
					x1 = x_intersection;
				if (x_intersection > x2)
					x2 = x_intersection;
			}
			++i;
		}
		if (x1 <= x2)
		{
			set_ivec2(&v1.coord, x1, y);
			set_ivec2(&v2.coord, x2, y);
			draw_line(man, v1, v2);
		}
		++y;
	}
	return ;
}

/* Barycentric weights for color blending */
static void	draw_full_triangle(t_man *man, t_vert *v)
{
	t_ivec2	min_coord;
	t_ivec2	max_coord;
	t_ivec2	p;
	float	denom;
	float	inv_denom;

	min_coord.x = min(min(v[0].coord.x, v[1].coord.x), v[2].coord.x);
	min_coord.y = min(min(v[0].coord.y, v[1].coord.y), v[2].coord.y);
	max_coord.x = max(max(v[0].coord.x, v[1].coord.x), v[2].coord.x);
	max_coord.y = max(max(v[0].coord.y, v[1].coord.y), v[2].coord.y);
	denom = (v[1].coord.y - v[2].coord.y) * (v[0].coord.x - v[2].coord.x)
		+ (v[2].coord.x - v[1].coord.x) * (v[0].coord.y - v[2].coord.y);
	inv_denom = 1.0f / denom;
	p.y = min_coord.y;
	while (p.y <= max_coord.y)
	{
		p.x = min_coord.x;
		while (p.x <= max_coord.x)
		{
			draw_blended_p(man, v, p, inv_denom);
			++p.x;
		}
		++p.y;
	}
	return ;
}

static void	draw_blended_p(t_man *man, t_vert *v, t_ivec2 p, float inv_denom)
{
	t_color	color;
	float	w[3];

	w[0] = ((v[1].coord.y - v[2].coord.y) * (p.x - v[2].coord.x)
			+ (v[2].coord.x - v[1].coord.x) * (p.y - v[2].coord.y)) * inv_denom;
	w[1] = ((v[2].coord.y - v[0].coord.y) * (p.x - v[2].coord.x)
			+ (v[0].coord.x - v[2].coord.x) * (p.y - v[2].coord.y)) * inv_denom;
	w[2] = 1.0f - w[0] - w[1];
	if (w[0] < 0 || w[1] < 0 || w[2] < 0)
		return ;
	color.r = v[0].color.r * w[0] + v[1].color.r * w[1] + v[2].color.r * w[2];
	color.g = v[0].color.g * w[0] + v[1].color.g * w[1] + v[2].color.g * w[2];
	color.b = v[0].color.b * w[0] + v[1].color.b * w[1] + v[2].color.b * w[2];
	color.a = v[0].color.a * w[0] + v[1].color.a * w[1] + v[2].color.a * w[2];
	draw_point(man, color, p.x, p.y);
	return ;
}
