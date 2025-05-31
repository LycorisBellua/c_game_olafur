#include "cimmerian.h"

static int	check_north_face(t_map *m, t_ray *r, int img_coord_x,
				int img_size_x);
static int	check_south_face(t_map *m, t_ray *r, int img_coord_x,
				int img_size_x);
static int	check_west_face(t_map *m, t_ray *r, int img_coord_x,
				int img_size_x);
static int	check_east_face(t_map *m, t_ray *r, int img_coord_x,
				int img_size_x);

int	is_corner(t_map *m, t_ray *r, int img_coord_x, int img_size_x)
{
	int	ten_percent;

	ten_percent = img_size_x * 0.1;
	if (img_coord_x >= ten_percent && img_coord_x < img_size_x - ten_percent)
		return (0);
	else if (m->cells[r->m_index.y][r->m_index.x].door
		|| m->cells[r->m_index.y][r->m_index.x].is_goal)
		return (1);
	else if (r->side == 1 && r->ray_dir.y > 0)
		return (check_north_face(m, r, img_coord_x, img_size_x));
	else if (r->side == 1 && r->ray_dir.y < 0)
		return (check_south_face(m, r, img_coord_x, img_size_x));
	else if (r->side == 0 && r->ray_dir.x > 0)
		return (check_west_face(m, r, img_coord_x, img_size_x));
	else if (r->side == 0 && r->ray_dir.x < 0)
		return (check_east_face(m, r, img_coord_x, img_size_x));
	return (0);
}

static int	check_north_face(t_map *m, t_ray *r, int img_coord_x,
	int img_size_x)
{
	t_ivec2	c;

	set_ivec2(&c, r->m_index.x, r->m_index.y);
	if (img_coord_x < img_size_x * 0.1)
	{
		if (c.x == m->size.x - 1 || c.y == 0
			|| !m->cells[c.y][c.x + 1].is_visible
			|| m->cells[c.y][c.x + 1].door
			|| m->cells[c.y][c.x + 1].portal
			|| m->cells[c.y][c.x + 1].is_goal
			|| m->cells[c.y - 1][c.x + 1].is_visible)
			return (1);
	}
	else if (img_coord_x >= img_size_x - img_size_x * 0.1)
	{
		if (c.x == 0 || c.y == 0
			|| !m->cells[c.y][c.x - 1].is_visible
			|| m->cells[c.y][c.x - 1].door
			|| m->cells[c.y][c.x - 1].portal
			|| m->cells[c.y][c.x - 1].is_goal
			|| m->cells[c.y - 1][c.x - 1].is_visible)
			return (1);
	}
	return (0);
}

static int	check_south_face(t_map *m, t_ray *r, int img_coord_x,
	int img_size_x)
{
	t_ivec2	c;

	set_ivec2(&c, r->m_index.x, r->m_index.y);
	if (img_coord_x < img_size_x * 0.1)
	{
		if (c.x == 0 || c.y == m->size.y - 1
			|| !m->cells[c.y][c.x - 1].is_visible
			|| m->cells[c.y][c.x - 1].door
			|| m->cells[c.y][c.x - 1].portal
			|| m->cells[c.y][c.x - 1].is_goal
			|| m->cells[c.y + 1][c.x - 1].is_visible)
			return (1);
	}
	else if (img_coord_x >= img_size_x - img_size_x * 0.1)
	{
		if (c.x == m->size.x - 1 || c.y == m->size.y - 1
			|| !m->cells[c.y][c.x + 1].is_visible
			|| m->cells[c.y][c.x + 1].door
			|| m->cells[c.y][c.x + 1].portal
			|| m->cells[c.y][c.x + 1].is_goal
			|| m->cells[c.y + 1][c.x + 1].is_visible)
			return (1);
	}
	return (0);
}

static int	check_west_face(t_map *m, t_ray *r, int img_coord_x, int img_size_x)
{
	t_ivec2	c;

	set_ivec2(&c, r->m_index.x, r->m_index.y);
	if (img_coord_x < img_size_x * 0.1)
	{
		if (c.x == 0 || c.y == 0
			|| !m->cells[c.y - 1][c.x].is_visible
			|| m->cells[c.y - 1][c.x].door
			|| m->cells[c.y - 1][c.x].portal
			|| m->cells[c.y - 1][c.x].is_goal
			|| m->cells[c.y - 1][c.x - 1].is_visible)
			return (1);
	}
	else if (img_coord_x >= img_size_x - img_size_x * 0.1)
	{
		if (c.x == 0 || c.y == m->size.y - 1
			|| !m->cells[c.y + 1][c.x].is_visible
			|| m->cells[c.y + 1][c.x].door
			|| m->cells[c.y + 1][c.x].portal
			|| m->cells[c.y + 1][c.x].is_goal
			|| m->cells[c.y + 1][c.x - 1].is_visible)
			return (1);
	}
	return (0);
}

static int	check_east_face(t_map *m, t_ray *r, int img_coord_x, int img_size_x)
{
	t_ivec2	c;

	set_ivec2(&c, r->m_index.x, r->m_index.y);
	if (img_coord_x < img_size_x * 0.1)
	{
		if (c.x == m->size.x - 1 || c.y == m->size.y - 1
			|| !m->cells[c.y + 1][c.x].is_visible
			|| m->cells[c.y + 1][c.x].door
			|| m->cells[c.y + 1][c.x].portal
			|| m->cells[c.y + 1][c.x].is_goal
			|| m->cells[c.y + 1][c.x + 1].is_visible)
			return (1);
	}
	else if (img_coord_x >= img_size_x - img_size_x * 0.1)
	{
		if (c.x == m->size.x - 1 || c.y == 0
			|| !m->cells[c.y - 1][c.x].is_visible
			|| m->cells[c.y - 1][c.x].door
			|| m->cells[c.y - 1][c.x].portal
			|| m->cells[c.y - 1][c.x].is_goal
			|| m->cells[c.y - 1][c.x + 1].is_visible)
			return (1);
	}
	return (0);
}
