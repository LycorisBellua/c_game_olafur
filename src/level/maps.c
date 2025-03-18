#include "cimmerian.h"

#define MAP_SIZE_X 24
#define MAP_SIZE_Y 24
#define PLAYER_POS_X 22
#define PLAYER_POS_Y 12

static void	init_minimap_values(t_man *man);
static void	set_map_cells(t_man *man);

static int		map_walls[] =
{
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,2,2,5,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1,
	1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1,
	1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,2,2,5,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,4,0,0,0,0,6,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
};

static int		map_buildings[] =
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

int	create_map(t_man *man)
{
	man->map = calloc(1, sizeof(t_map));
	if (!man->map)
		return (0);
	set_ivec2(&man->map->size, MAP_SIZE_X, MAP_SIZE_Y);
	set_vec2(&man->map->start_pos, PLAYER_POS_X, PLAYER_POS_Y);
	man->map->start_dir = get_cardinal_dir('N');
	man->map->has_echolocation = 1;
	man->map->dof = man->map->has_echolocation ? 0 : 20;
	man->map->fog_width = get_fog_width(man->map->dof);
	man->map->fog_color = get_color_rgba(17, 15, 35, 255);
	init_minimap_values(man);
	compose_skybox(man->map, get_image(man, "src_skybox"), man->map->fog_color);
	if (!man->map->skybox)
	{
		free_map(man);
		return (0);
	}
	compose_background(man);
	if (!man->map->background || !set_cell_array(man)
		|| !set_sprite_array(man, NBR_SPR))
	{
		free_map(man);
		return (0);
	}
	return (1);
}

void	free_map(t_man *man)
{
	if (!man->map)
		return ;
	free(man->map->cells);
	free_image(man->map->skybox, free);
	free_png(man->map->background);
	free_sprite_array(man);
	free(man->map);
	man->map = 0;
	return ;
}

static void	init_minimap_values(t_man *man)
{
	set_ivec2(&man->map->minimap_offset, 563, 7);
	set_ivec2(&man->map->minimap_center, 599, 43);
	man->map->minimap_radius = 36;
	man->map->minimap_zoom = 9;
	man->map->minimap_cell_amount = man->map->minimap_radius
		/ man->map->minimap_zoom * 2;
	return ;
}

static int	set_map_cells(t_man *man)
{
	int		i;
	t_img	*img;

	man->map->cells = calloc(man->map->size.x * man->map->size.y,
		sizeof(t_cell));
	if (!man->map->cells)
		return (0);
	i = 0;
	while (i < man->map->size.x * man->map->size.y)
	{
		man->map->cells[i].is_obstacle = map_walls[i] > 0;
		man->map->cells[i].is_door = map_walls[i] == 5;
		man->map->cells[i].is_goal = map_walls[i] == 6;
		man->map->cells[i].is_indoors = map_buildings[i];
		if (man->map->cells[i].is_obstacle)
			man->map->cells[i].height = 1.0;
		if (map_walls[i] == 6)
			man->map->cells[i].height = 3.0;
		if (map_buildings[i])
		{
			man->map->cells[i].tex_floor = get_image(man, "tex_floor_indoors");
			man->map->cells[i].tex_ceiling = get_image(man, "tex_ceiling");
		}
		else
			man->map->cells[i].tex_floor = get_image(man, "tex_floor");
		img = 0;
		if (map_walls[i] == 1)
			img = get_image(man, "tex_wall_01");
		else if (map_walls[i] == 2)
			img = get_image(man, "tex_wall_02");
		else if (map_walls[i] == 3)
			img = get_image(man, "tex_wall_03");
		else if (map_walls[i] == 4)
			img = get_image(man, "tex_wall_04");
		else if (map_walls[i] == 5)
			img = get_image(man, "tex_doors");
		else if (map_walls[i] == 6)
			img = get_image(man, "solid_color");
		man->map->cells[i].tex_north = img;
		man->map->cells[i].tex_east = img;
		man->map->cells[i].tex_south = img;
		man->map->cells[i].tex_west = img;
		++i;
	}
	return (1);
}
