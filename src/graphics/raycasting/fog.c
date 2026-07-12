#include "olafur.h"

void	init_fog(t_man *man)
{
	man->dof = DEFAULT_DOF;
	man->fog_width = get_fog_width(man->dof);
	return ;
}

float	get_fog_width(float dof)
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
