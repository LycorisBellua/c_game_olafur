#include "cimmerian.h"

static void	set_north_tex(t_ray *r, t_cell *c);
static void	set_south_tex(t_ray *r, t_cell *c);
static void	set_west_tex(t_ray *r, t_cell *c);
static void	set_east_tex(t_ray *r, t_cell *c);

void	set_texture_and_is_see_through(t_ray *r, t_cell *c)
{
	r->tex = 0;
	r->tex_portal = 0;
	if (r->side == 1 && r->ray_dir.y > 0)
		set_north_tex(r, c);
	else if (r->side == 1 && r->ray_dir.y < 0)
		set_south_tex(r, c);
	else if (r->side == 0 && r->ray_dir.x > 0)
		set_west_tex(r, c);
	else if (r->side == 0 && r->ray_dir.x < 0)
		set_east_tex(r, c);
	r->is_see_through = !r->tex || r->tex->is_see_through[r->tex->cycle_index];
	return ;
}

static void	set_north_tex(t_ray *r, t_cell *c)
{
	r->tex = c->tex_north;
	if (c->portal && c->portal->src_cardinal == 'N')
		r->tex_portal = c->portal->tex;
	return ;
}

static void	set_south_tex(t_ray *r, t_cell *c)
{
	r->tex = c->tex_south;
	if (c->portal && c->portal->src_cardinal == 'S')
		r->tex_portal = c->portal->tex;
	return ;
}

static void	set_west_tex(t_ray *r, t_cell *c)
{
	r->tex = c->tex_west;
	if (c->portal && c->portal->src_cardinal == 'W')
		r->tex_portal = c->portal->tex;
	return ;
}

static void	set_east_tex(t_ray *r, t_cell *c)
{
	r->tex = c->tex_east;
	if (c->portal && c->portal->src_cardinal == 'E')
		r->tex_portal = c->portal->tex;
	return ;
}
