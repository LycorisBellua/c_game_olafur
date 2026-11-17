#include "olafur.h"

void	init_fog(t_man *man)
{
	man->dof = DEFAULT_DOF;
	man->fog_width = get_fog_width(man->dof);
	return ;
}

float	get_fog_width(float dof)
{
	if (dof < 2.0f)
		return (1.0f);
	else if (dof < 3.8f)
		return (0.80f - (dof - 2.0f) / 2 * 0.5f);
	else if (dof < 5)
		return (0.40f);
	else if (dof < 8)
		return (0.30f);
	else if (dof < 8.5f)
		return (0.2f);
	else if (dof < 9)
		return (0.15f);
	return (0.1f);
}
