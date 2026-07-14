#include "olafur.h"

static void		draw_full_triangle(t_man *man, t_vert *v);
static void		draw_blended_p(t_man *man, t_vert *v, t_ivec2 p,
					float inv_denom);
static void		fill_polygon(t_man *man, t_vert arr[], int len);
static t_color	idw_color(t_vert arr[], int len, t_ivec2 p);

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

/*
	Fills a polygon of any kind - convex, concave, or self-intersecting. It 
	scans line by line using the nonzero-winding rule (so self-intersections 
	fill solidly) and writes every pixel exactly once, so translucent fills 
	never double-blend along a seam. Each pixel's colour is an inverse-distance 
	weighted average of the vertices: a convex combination that never over- or 
	undershoots and reproduces each vertex colour exactly. A triangle keeps 
	exact barycentric interpolation, which is the natural linear gradient there.
*/
void	draw_shape_full(t_man *man, t_vert arr[], int len)
{
	if (len < 3)
		draw_shape(man, arr, len);
	else if (len == 3)
		draw_full_triangle(man, arr);
	else
		fill_polygon(man, arr, len);
	return ;
}

static t_color	idw_color(t_vert arr[], int len, t_ivec2 p)
{
	double	acc[4];
	double	sum;
	double	w;
	int		i;
	t_color	c;

	acc[0] = 0;
	acc[1] = 0;
	acc[2] = 0;
	acc[3] = 0;
	sum = 0;
	i = -1;
	while (++i < len)
	{
		w = (double)(arr[i].coord.x - p.x) * (arr[i].coord.x - p.x)
			+ (double)(arr[i].coord.y - p.y) * (arr[i].coord.y - p.y);
		if (w == 0)
			return (arr[i].color);
		w = 1.0 / w;
		sum += w;
		acc[0] += w * arr[i].color.r;
		acc[1] += w * arr[i].color.g;
		acc[2] += w * arr[i].color.b;
		acc[3] += w * arr[i].color.a;
	}
	c.r = acc[0] / sum;
	c.g = acc[1] / sum;
	c.b = acc[2] / sum;
	c.a = acc[3] / sum;
	return (c);
}

/*
	Nonzero-winding scanline fill. For each row, gathers the x where edges 
	cross it, each tagged with the edge's vertical direction (+1 down, -1 up), 
	sorts them by x, then sweeps left to right accumulating the winding number 
	and fills every interval where it is non-zero. Self-intersecting shapes 
	fill solidly (no even-odd holes), and each pixel is still written exactly 
	once.
*/
static void	fill_polygon(t_man *man, t_vert arr[], int len)
{
	float	*xs;
	int		*dir;
	t_ivec2	p;
	int		ymin;
	int		ymax;
	int		n;
	int		i;
	int		j;
	int		d;
	int		wind;
	int		x1;
	float	kx;
	int		kd;

	xs = malloc(sizeof(float) * len);
	dir = malloc(sizeof(int) * len);
	if (!xs || !dir)
	{
		free(xs);
		free(dir);
		return ;
	}
	ymin = arr[0].coord.y;
	ymax = arr[0].coord.y;
	i = 0;
	while (++i < len)
	{
		ymin = imin(ymin, arr[i].coord.y);
		ymax = imax(ymax, arr[i].coord.y);
	}
	p.y = ymin - 1;
	while (++p.y <= ymax)
	{
		n = 0;
		i = -1;
		while (++i < len)
		{
			j = (i + 1) % len;
			d = 0;
			if (arr[i].coord.y <= p.y && arr[j].coord.y > p.y)
				d = 1;
			else if (arr[j].coord.y <= p.y && arr[i].coord.y > p.y)
				d = -1;
			if (d != 0)
			{
				xs[n] = arr[i].coord.x + (float)(p.y - arr[i].coord.y)
					* (arr[j].coord.x - arr[i].coord.x)
					/ (arr[j].coord.y - arr[i].coord.y);
				dir[n++] = d;
			}
		}
		i = 0;
		while (++i < n)
		{
			kx = xs[i];
			kd = dir[i];
			j = i - 1;
			while (j >= 0 && xs[j] > kx)
			{
				xs[j + 1] = xs[j];
				dir[j + 1] = dir[j];
				--j;
			}
			xs[j + 1] = kx;
			dir[j + 1] = kd;
		}
		wind = 0;
		i = -1;
		while (++i < n)
		{
			wind += dir[i];
			if (wind == 0 || i + 1 >= n)
				continue ;
			p.x = (int)xs[i];
			if (xs[i] > (float)p.x)
				++p.x;
			x1 = (int)xs[i + 1];
			if (xs[i + 1] < (float)x1)
				--x1;
			while (p.x <= x1)
			{
				draw_point(man, idw_color(arr, len, p), p.x, p.y);
				++p.x;
			}
		}
	}
	free(xs);
	free(dir);
	return ;
}

/* Barycentric weights for color blending (exact linear gradient) */
static void	draw_full_triangle(t_man *man, t_vert *v)
{
	t_ivec2	min_coord;
	t_ivec2	max_coord;
	t_ivec2	p;
	float	denom;
	float	inv_denom;

	min_coord.x = imin(imin(v[0].coord.x, v[1].coord.x), v[2].coord.x);
	min_coord.y = imin(imin(v[0].coord.y, v[1].coord.y), v[2].coord.y);
	max_coord.x = imax(imax(v[0].coord.x, v[1].coord.x), v[2].coord.x);
	max_coord.y = imax(imax(v[0].coord.y, v[1].coord.y), v[2].coord.y);
	denom = (v[1].coord.y - v[2].coord.y) * (v[0].coord.x - v[2].coord.x)
		+ (v[2].coord.x - v[1].coord.x) * (v[0].coord.y - v[2].coord.y);
	if (denom == 0)
		return ;
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
