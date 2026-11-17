#include "olafur.h"

static void		collect_crossings(t_scanline *s, t_vert arr[], int len);
static void		sort_crossings(t_scanline *s);
static void		fill_spans(t_man *man, t_scanline *s, t_vert arr[], int len);
static t_color	idw_color(t_vert arr[], int len, t_ivec2 p);

/*
	Nonzero-winding scanline fill. For each row, gathers the x where edges 
	cross it, each tagged with the edge's vertical direction (+1 down, -1 up), 
	sorts them by x, then sweeps left to right accumulating the winding number 
	and fills every interval where it is non-zero. Self-intersecting shapes 
	fill solidly (no even-odd holes), and each pixel is still written exactly 
	once.
*/
void	fill_polygon(t_man *man, t_vert arr[], int len)
{
	t_scanline	s;
	int			ymin;
	int			ymax;
	int			i;

	s.xs = malloc(sizeof(float) * len);
	s.dir = malloc(sizeof(int) * len);
	ymin = arr[0].coord.y;
	ymax = arr[0].coord.y;
	i = 0;
	while (++i < len)
	{
		ymin = imin(ymin, arr[i].coord.y);
		ymax = imax(ymax, arr[i].coord.y);
	}
	s.p.y = ymin - 1;
	while (s.xs && s.dir && ++s.p.y <= ymax)
	{
		collect_crossings(&s, arr, len);
		sort_crossings(&s);
		fill_spans(man, &s, arr, len);
	}
	free(s.xs);
	free(s.dir);
	return ;
}

static void	collect_crossings(t_scanline *s, t_vert arr[], int len)
{
	int	i;
	int	j;
	int	d;

	s->n = 0;
	i = -1;
	while (++i < len)
	{
		j = (i + 1) % len;
		d = 0;
		if (arr[i].coord.y <= s->p.y && arr[j].coord.y > s->p.y)
			d = 1;
		else if (arr[j].coord.y <= s->p.y && arr[i].coord.y > s->p.y)
			d = -1;
		if (d != 0)
		{
			s->xs[s->n] = arr[i].coord.x + (float)(s->p.y - arr[i].coord.y)
				* (arr[j].coord.x - arr[i].coord.x)
				/ (arr[j].coord.y - arr[i].coord.y);
			s->dir[s->n++] = d;
		}
	}
	return ;
}

static void	sort_crossings(t_scanline *s)
{
	float	kx;
	int		kd;
	int		i;
	int		j;

	i = 0;
	while (++i < s->n)
	{
		kx = s->xs[i];
		kd = s->dir[i];
		j = i - 1;
		while (j >= 0 && s->xs[j] > kx)
		{
			s->xs[j + 1] = s->xs[j];
			s->dir[j + 1] = s->dir[j];
			--j;
		}
		s->xs[j + 1] = kx;
		s->dir[j + 1] = kd;
	}
	return ;
}

static void	fill_spans(t_man *man, t_scanline *s, t_vert arr[], int len)
{
	int	wind;
	int	x1;
	int	i;

	wind = 0;
	i = -1;
	while (++i < s->n)
	{
		wind += s->dir[i];
		if (wind == 0 || i + 1 >= s->n)
			continue ;
		s->p.x = (int)s->xs[i];
		if (s->xs[i] > (float)s->p.x)
			++s->p.x;
		x1 = (int)s->xs[i + 1];
		if (s->xs[i + 1] < (float)x1)
			--x1;
		while (s->p.x <= x1)
		{
			draw_point(man, idw_color(arr, len, s->p), s->p.x, s->p.y);
			++s->p.x;
		}
	}
	return ;
}

/*
	Inverse-distance weighted average of the vertex colours. acc[4] holds the 
	sum of the weights.
*/
static t_color	idw_color(t_vert arr[], int len, t_ivec2 p)
{
	float	acc[5];
	float	w;
	int		i;

	i = -1;
	while (++i < 5)
		acc[i] = 0;
	i = -1;
	while (++i < len)
	{
		w = (float)(arr[i].coord.x - p.x) * (arr[i].coord.x - p.x)
			+ (float)(arr[i].coord.y - p.y) * (arr[i].coord.y - p.y);
		if (w == 0)
			return (arr[i].color);
		w = 1.0f / w;
		acc[0] += w * arr[i].color.r;
		acc[1] += w * arr[i].color.g;
		acc[2] += w * arr[i].color.b;
		acc[3] += w * arr[i].color.a;
		acc[4] += w;
	}
	return (get_color_rgba(acc[0] / acc[4], acc[1] / acc[4],
		acc[2] / acc[4], acc[3] / acc[4]));
}
