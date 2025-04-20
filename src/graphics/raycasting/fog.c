#include "cimmerian.h"

void	init_fog(t_man *man)
{
	man->echolocation = ECHOLOCATION;
	if (!man->echolocation)
		man->dof = DEFAULT_DOF;
	man->fog_width = get_fog_width(man->dof);
	return ;
}

void	update_dof(t_man *man, double increment)
{
	double	prev_dof;

	prev_dof = man->dof;
	man->dof = fclamp(man->dof + increment, 0, 30);
	if (prev_dof != man->dof)
		man->fog_width = get_fog_width(man->dof);
	return ;
}

double	get_fog_width(double dof)
{
	if (dof < 2.0)
		return (1.0);
	else if (dof >= 2.0 && dof < 3.79)
		return (0.80 - (dof - 2.0) / 2 * 0.5);
	else if (dof >= 3.8 && dof < 5)
		return (0.40);
	else if (dof >= 5 && dof < 8)
		return (0.30);
	else if (dof >= 8 && dof < 8.5)
		return (0.2);
	else if (dof >= 8.5 && dof < 9)
		return (0.15);
	return (0.1);
}

/* In `height`, x and y are used as "start" and "end" */
void	apply_wall_shadow(t_color *wall, t_color c, int y, t_ivec2 height)
{
	double	gradient_strength;
	double	edge_dist;
	double	factor;
	int		middle_y;

	gradient_strength = 0.6;
	middle_y = (height.x + height.y) / 2;
	if (y < middle_y)
		edge_dist = (double)(y - height.x) / (middle_y - height.x);
	else
		edge_dist = (double)(height.y - y) / (height.y - middle_y);
	if (edge_dist < 0.0 || edge_dist > 1.0)
		return ;
	factor = pow(1.0 - edge_dist, 2.0) * gradient_strength;
	wall->r = (1.0 - factor) * wall->r + factor * c.r;
	wall->g = (1.0 - factor) * wall->g + factor * c.g;
	wall->b = (1.0 - factor) * wall->b + factor * c.b;
	if (wall->a > 0)
		wall->a = (1.0 - factor) * wall->a + factor * c.a;
	return ;
}

void	apply_corner_shadow(t_color *wall, t_color c, int img_coord_x,
	int img_size_x)
{
	int		ten_percent;
	int		corner_boundary;
	double	dist_from_corner;
	double	intensity;

	ten_percent = img_size_x * 0.1;
	corner_boundary = img_size_x - 1;
	if (img_coord_x < ten_percent)
		corner_boundary = 0;
	dist_from_corner = fabs((double)img_coord_x - corner_boundary);
	intensity = fmax(0.0, 1.0 - (dist_from_corner / ten_percent)) * 0.4;
	wall->r = wall->r + intensity * (c.r - wall->r);
	wall->g = wall->g + intensity * (c.g - wall->g);
	wall->b = wall->b + intensity * (c.b - wall->b);
	return ;
}

void	apply_wall_fog(t_color *wall, t_color c, double dist, double dof)
{
	double	factor;

	factor = fmin(dist / dof, 1);
	wall->r = wall->r + factor * (c.r - wall->r);
	wall->g = wall->g + factor * (c.g - wall->g);
	wall->b = wall->b + factor * (c.b - wall->b);
	return ;
}
