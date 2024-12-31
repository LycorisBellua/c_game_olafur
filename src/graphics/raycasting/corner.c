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
	else if (m->cells[r->m_index.y * m->size.x + r->m_index.x].is_door)
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
	int	x;
	int	y;
	int	ten_percent;

	x = r->m_index.x;
	y = r->m_index.y;
	ten_percent = img_size_x * 0.1;
	if (img_coord_x < ten_percent)
	{
		if (x == m->size.x - 1 || y == 0
			|| !m->cells[y * m->size.x + (x + 1)].is_obstacle
			|| m->cells[y * m->size.x + (x + 1)].is_door
			|| m->cells[(y - 1) * m->size.x + (x + 1)].is_obstacle)
			return (1);
	}
	else if (img_coord_x >= img_size_x - ten_percent)
	{
		if (x == 0 || y == 0
			|| !m->cells[y * m->size.x + (x - 1)].is_obstacle
			|| m->cells[y * m->size.x + (x - 1)].is_door
			|| m->cells[(y - 1) * m->size.x + (x - 1)].is_obstacle)
			return (1);
	}
	return (0);
}

static int	check_south_face(t_map *m, t_ray *r, int img_coord_x,
	int img_size_x)
{
	int	x;
	int	y;
	int	ten_percent;

	x = r->m_index.x;
	y = r->m_index.y;
	ten_percent = img_size_x * 0.1;
	if (img_coord_x < ten_percent)
	{
		if (x == 0 || y == m->size.y - 1
			|| !m->cells[y * m->size.x + (x - 1)].is_obstacle
			|| m->cells[y * m->size.x + (x - 1)].is_door
			|| m->cells[(y + 1) * m->size.x + (x - 1)].is_obstacle)
			return (1);
	}
	else if (img_coord_x >= img_size_x - ten_percent)
	{
		if (x == m->size.x - 1 || y == m->size.y - 1
			|| !m->cells[y * m->size.x + (x + 1)].is_obstacle
			|| m->cells[y * m->size.x + (x + 1)].is_door
			|| m->cells[(y + 1) * m->size.x + (x + 1)].is_obstacle)
			return (1);
	}
	return (0);
}

static int	check_west_face(t_map *m, t_ray *r, int img_coord_x, int img_size_x)
{
	int	x;
	int	y;
	int	ten_percent;

	x = r->m_index.x;
	y = r->m_index.y;
	ten_percent = img_size_x * 0.1;
	if (img_coord_x < ten_percent)
	{
		if (x == 0 || y == 0
			|| !m->cells[(y - 1) * m->size.x + x].is_obstacle
			|| m->cells[(y - 1) * m->size.x + x].is_door
			|| m->cells[(y - 1) * m->size.x + (x - 1)].is_obstacle)
			return (1);
	}
	else if (img_coord_x >= img_size_x - ten_percent)
	{
		if (x == 0 || y == m->size.y - 1
			|| !m->cells[(y + 1) * m->size.x + x].is_obstacle
			|| m->cells[(y + 1) * m->size.x + x].is_door
			|| m->cells[(y + 1) * m->size.x + (x - 1)].is_obstacle)
			return (1);
	}
	return (0);
}

static int	check_east_face(t_map *m, t_ray *r, int img_coord_x, int img_size_x)
{
	int	x;
	int	y;
	int	ten_percent;

	x = r->m_index.x;
	y = r->m_index.y;
	ten_percent = img_size_x * 0.1;
	if (img_coord_x < ten_percent)
	{
		if (x == m->size.x - 1 || y == m->size.y - 1
			|| !m->cells[(y + 1) * m->size.x + x].is_obstacle
			|| m->cells[(y + 1) * m->size.x + x].is_door
			|| m->cells[(y + 1) * m->size.x + (x + 1)].is_obstacle)
			return (1);
	}
	else if (img_coord_x >= img_size_x - ten_percent)
	{
		if (x == m->size.x - 1 || y == 0
			|| !m->cells[(y - 1) * m->size.x + x].is_obstacle
			|| m->cells[(y - 1) * m->size.x + x].is_door
			|| m->cells[(y - 1) * m->size.x + (x + 1)].is_obstacle)
			return (1);
	}
	return (0);
}
