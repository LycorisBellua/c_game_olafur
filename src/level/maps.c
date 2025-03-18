#include "cimmerian.h"

#define MAP_SIZE_X 24
#define MAP_SIZE_Y 24
#define PLAYER_POS_X 22
#define PLAYER_POS_Y 12

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
	int	i;

	man->map = calloc(1, sizeof(t_map));
	if (!man->map)
		return (0);
	man->map->size.x = MAP_SIZE_X;
	man->map->size.y = MAP_SIZE_Y;
	man->map->start_pos.x = PLAYER_POS_X;
	man->map->start_pos.y = PLAYER_POS_Y;
	man->map->start_dir = get_cardinal_dir('N');
	man->map->has_echolocation = 1;
	man->map->dof = man->map->has_echolocation ? 0 : 20;
	man->map->fog_width = get_fog_width(man->map->dof);
	man->map->fog_color = get_color_rgba(17, 15, 35, 255);
	set_ivec2(&man->map->minimap_offset, 563, 7);
	set_ivec2(&man->map->minimap_center, 599, 43);
	man->map->minimap_radius = 36;
	man->map->minimap_zoom = 9;
	man->map->minimap_cell_amount = man->map->minimap_radius / man->map->minimap_zoom * 2;
	compose_skybox(man->map, get_image(man, "src_skybox"), man->map->fog_color);
	if (!man->map->skybox)
	{
		free_map(man);
		return (0);
	}
	compose_background(man);
	if (!man->map->background)
	{
		free_map(man);
		return (0);
	}
	man->map->cells = malloc(man->map->size.x * man->map->size.y * sizeof(t_cell));
	if (!man->map->cells)
	{
		free_map(man);
		return (0);
	}
	t_img *img;
	i = 0;
	while (i < man->map->size.x * man->map->size.y)
	{
		man->map->cells[i].is_obstacle = map_walls[i] > 0;
		man->map->cells[i].is_door = map_walls[i] == 5;
		man->map->cells[i].is_goal = map_walls[i] == 6;
		man->map->cells[i].is_indoors = map_buildings[i];
		man->map->cells[i].height = !man->map->cells[i].is_obstacle ? 0 : 1.0;
		if (map_walls[i] == 6)
			man->map->cells[i].height = 3.0;
		man->map->cells[i].tex_floor = map_buildings[i] ? get_image(man, "tex_floor_indoors") : get_image(man, "tex_floor");
		man->map->cells[i].tex_ceiling = map_buildings[i] ? get_image(man, "tex_ceiling") : 0;
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
	if (!set_sprite_array(man, NBR_SPR))
	{
		free_map(man);
		return (0);
	}
	return (1);
}
