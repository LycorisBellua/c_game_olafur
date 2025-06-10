#include "cimmerian.h"

static void	update_background_offset(t_man *man);

void	rotate_player(t_man *man, double angle)
{
	t_vec2	dir;
	t_vec2	plane;

	dir = rotate_vec2(man->player.dir, angle);
	plane = rotate_vec2(man->player.plane, angle);
	set_vec2(&man->player.dir, dir.x, dir.y);
	set_vec2(&man->player.plane, plane.x, plane.y);
	update_background_offset(man);
	return ;
}

static void	update_background_offset(t_man *man)
{
	t_map	*map;
	double	angle;

	map = man->maps[man->curr_map];
	if (!map || !map->background)
		return ;
	angle = get_angle_from_dir(man->player.dir) + RAD_90;
	map->background_offset = angle / RAD_360 * map->background->size.x;
	return ;
}
