#include "olafur.h"

static int	should_draw(t_man *man, t_ivec2 point);

void	draw_rect_minimap(t_man *man, t_vert v)
{
	t_ivec2			point;
	t_ivec2			size;

	set_ivec2(&size, man->minimap_zoom - 1, man->minimap_zoom - 1);
	if (!size.x)
		set_ivec2(&size, 1, 1);
	if (v.coord.y <= 0)
		--size.y;
	point.y = v.coord.y - 1;
	while (++point.y <= v.coord.y + size.y - 1)
	{
		point.x = v.coord.x - 1;
		while (++point.x <= v.coord.x + size.x - 1)
		{
			if (should_draw(man, point))
				draw_point(man, v.color, point.x, point.y);
		}
	}
	return ;
}

static int	should_draw(t_man *man, t_ivec2 point)
{
	t_vec2	delta;
	float	dist_sq;
	float	d;

	delta.x = point.x - man->minimap_center.x;
	delta.y = point.y - man->minimap_center.y;
	dist_sq = delta.x * delta.x + delta.y * delta.y;
	if (dist_sq > man->minimap_radius_sq + man->minimap_zoom)
		return (0);
	d = dot(delta, man->player.dir);
	if (d <= 0)
		return (0);
	return (d * d >= man->minimap_half_90_deg_cos_sq * dist_sq);
}
