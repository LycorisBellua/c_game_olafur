#include "olafur.h"

static int	should_draw(t_man *man, t_ivec2 point, int circle_radius_sq);

void	draw_rect_minimap(t_man *man, t_vert v)
{
	t_ivec2			point;
	t_ivec2			size;
	int				circle_radius_sq;

	set_ivec2(&size, man->minimap_zoom - 1, man->minimap_zoom - 1);
	if (!size.x)
		set_ivec2(&size, 1, 1);
	if (v.coord.y <= 0)
		--size.y;
	circle_radius_sq = man->minimap_radius * man->minimap_radius;
	point.y = v.coord.y - 1;
	while (++point.y <= v.coord.y + size.y - 1)
	{
		point.x = v.coord.x - 1;
		while (++point.x <= v.coord.x + size.x - 1)
		{
			if (should_draw(man, point, circle_radius_sq))
				draw_point(man, v.color, point.x, point.y);
		}
	}
	return ;
}

static int	should_draw(t_man *man, t_ivec2 point, int circle_radius_sq)
{
	t_vec2	delta;
	double	len;

	delta.x = point.x - man->minimap_center.x;
	delta.y = point.y - man->minimap_center.y;
	if (delta.x * delta.x + delta.y * delta.y > circle_radius_sq
		+ man->minimap_zoom)
		return (0);
	len = sqrt_approx(delta.x * delta.x + delta.y * delta.y);
	if (len > 0)
	{
		delta.x /= len;
		delta.y /= len;
	}
	return (dot(delta, man->player.dir) >= man->minimap_half_90_deg_cos);
}
