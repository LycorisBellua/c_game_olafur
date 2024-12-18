#include "cimmerian.h"

void	update_dof(t_map *m, double increment)
{
	m->dof = f_clamp(m->dof + increment, 0, 30);
	m->fog_width = get_fog_width(m->dof);
	update_background(m, m->img[0]);
	return ;
}

double	get_fog_width(double dof)
{
	if (dof < 2.0)
		return (1.0);
	else if (dof >= 2.0 && dof < 3.79)
		return (0.75 - (dof - 2.0) / 2 * 0.5);
	else if (dof >= 3.8 && dof < 5)
		return (0.3);
	else if (dof >= 5 && dof < 8)
		return (0.25);
	else if (dof >= 8 && dof < 8.5)
		return (0.2);
	else if (dof >= 8.5 && dof < 9)
		return (0.15);
	return (0.1);
}

void	apply_wall_fog(t_color *wall, t_color fog, double dist, double dof)
{
	double	factor;

	factor = f_min(dist / dof, 1);
	wall->r = wall->r + factor * (fog.r - wall->r);
	wall->g = wall->g + factor * (fog.g - wall->g);
	wall->b = wall->b + factor * (fog.b - wall->b);
	return ;
}
