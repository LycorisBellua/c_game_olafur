#include "olafur.h"

static void		set_transform(t_man *man, t_portal *portal);
static t_vec2	compute_start_pos(t_portal *portal, t_vec2 player_pos);
static t_vec2	compute_entry_dir(t_vec2 player_pos, t_ivec2 portal_pos);

void	portal_routine(t_man *man)
{
	t_portal	*portal;
	int			map_index;

	open_and_close_portals(man);
	portal = get_portal(man);
	if (!portal)
		return ;
	if (!portal->path_dst_map)
	{
		change_game_state(man, GAME_STATE_FAILURE);
		return ;
	}
	map_index = add_map(man, portal->path_dst_map);
	if (map_index < 0)
		return ;
	man->curr_map = map_index;
	man->player.is_in_portal = 1;
	set_transform(man, portal);
	if (man->echolocation)
		update_dof(man, -30);
	return ;
}

/*
	Fix the cardinal direction indicated in the map file so that it respects 
	the player's intent (e.g.: if the player should face south after exiting 
	the portal, but is moving backwards into said portal, change the direction 
	into north).

	Then, change the direction back to what the map file asked, in case the 
	player transform is ever reset.
*/
static void	set_transform(t_man *man, t_portal *portal)
{
	t_vec2	exit_pos;
	t_vec2	exit_dir;
	t_vec2	entry_dir;
	float	player_angle;
	float	entry_angle;
	float	relative_angle;

	exit_pos = man->maps[man->curr_map]->start_pos;
	exit_dir = man->maps[man->curr_map]->start_dir;
	if (portal->dst_cardinal && portal->trigger_opposite)
		exit_dir = get_dir_from_cardinal(opp_cardinal(portal->dst_cardinal));
	else if (portal->dst_cardinal)
		exit_dir = get_dir_from_cardinal(portal->dst_cardinal);
	if (portal->override_start_pos)
	{
		exit_pos = compute_start_pos(portal, man->player.pos);
		entry_dir = compute_entry_dir(man->player.pos, portal->src_pos);
		player_angle = get_angle_from_dir(man->player.dir);
		entry_angle = get_angle_from_dir(entry_dir);
		relative_angle = norm_angle(player_angle - entry_angle);
		exit_dir = rotate_vec2(exit_dir, relative_angle);
	}
	set_player_transform(man, exit_pos, exit_dir);
	return ;
}

static t_vec2	compute_start_pos(t_portal *portal, t_vec2 player_pos)
{
	t_vec2	p;
	float	offset;
	float	depth;
	t_vec2	dir;

	offset = 0.5;
	if (portal->src_cardinal == 'N' || portal->src_cardinal == 'S')
		offset = player_pos.x - portal->src_pos.x;
	else if (portal->src_cardinal == 'E' || portal->src_cardinal == 'W')
		offset = player_pos.y - portal->src_pos.y;
	offset = fclamp(offset, 0.0, 1.0);
	depth = 0.1;
	if (!portal->is_corridor || portal->trigger_opposite)
		depth += 0.5;
	dir = get_dir_from_cardinal(portal->dst_cardinal);
	set_vec2(&p, portal->dst_pos.x + 0.5, portal->dst_pos.y + 0.5);
	if (portal->is_corridor && !portal->trigger_opposite)
		set_vec2(&p, p.x - dir.x * (0.5 - depth), p.y - dir.y * (0.5 - depth));
	else
		set_vec2(&p, p.x + dir.x * depth, p.y + dir.y * depth);
	if (!dir.x)
		p.x = portal->dst_pos.x + offset;
	else
		p.y = portal->dst_pos.y + offset;
	return (p);
}

static t_vec2	compute_entry_dir(t_vec2 player_pos, t_ivec2 portal_pos)
{
	t_vec2	entry_dir;
	t_vec2	diff;

	set_vec2(&entry_dir, 0, 0);
	diff.x = player_pos.x - (portal_pos.x + 0.5);
	diff.y = player_pos.y - (portal_pos.y + 0.5);
	if (fabsf(diff.x) > fabsf(diff.y))
	{
		if (diff.x > 0)
			entry_dir.x = -1;
		else
			entry_dir.x = 1;
	}
	else
	{
		if (diff.y > 0)
			entry_dir.y = -1;
		else
			entry_dir.y = 1;
	}
	return (entry_dir);
}
