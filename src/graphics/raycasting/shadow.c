#include "olafur.h"

/* In `height`, x and y are used as "start" and "end" */
int	wall_shadow_factor8(t_ray *r, int y)
{
	float	edge_dist;
	float	falloff;

	if (y < r->shadow_middle_y)
		edge_dist = (y - r->unclamped_line_height.x) * r->shadow_inv_upper;
	else
		edge_dist = (r->unclamped_line_height.y - y) * r->shadow_inv_lower;
	if (edge_dist < 0.0f || edge_dist > 1.0f)
		return (0);
	falloff = 1.0f - edge_dist;
	return ((int)(falloff * falloff * 0.6f * 256.0f));
}

void	set_wall_shadow_params(t_ray *r)
{
	r->shadow_middle_y
		= (r->unclamped_line_height.x + r->unclamped_line_height.y) / 2;
	r->shadow_inv_upper
		= 1.0f / (r->shadow_middle_y - r->unclamped_line_height.x);
	r->shadow_inv_lower
		= 1.0f / (r->unclamped_line_height.y - r->shadow_middle_y);
}

float	corner_intensity_of(t_ray *r, t_img *tex, int tex_x)
{
	int		ten_percent;
	int		corner_boundary;
	float	dist_from_corner;
	float	intensity;

	if (!is_corner(r->m, r, tex_x, tex->size.x))
		return (0.0f);
	ten_percent = tex->size.x * 0.1;
	if (ten_percent <= 0)
		return (0.0f);
	corner_boundary = tex->size.x - 1;
	if (tex_x < ten_percent)
		corner_boundary = 0;
	dist_from_corner = fabsf((float)tex_x - corner_boundary);
	intensity = 1.0f - (dist_from_corner / ten_percent);
	if (intensity < 0.0f)
		intensity = 0.0f;
	return (intensity * 0.4f);
}

void	apply_fog_factor(t_color *wall, t_color c, int factor8)
{
	if (!factor8)
		return ;
	wall->r = ((256 - factor8) * wall->r + factor8 * c.r) >> 8;
	wall->g = ((256 - factor8) * wall->g + factor8 * c.g) >> 8;
	wall->b = ((256 - factor8) * wall->b + factor8 * c.b) >> 8;
	return ;
}

float	fog_factor_of(float dist, float dof)
{
	float	factor;

	factor = dist / dof;
	if (!(factor < 1.0f))
		factor = 1.0f;
	return (factor);
}
