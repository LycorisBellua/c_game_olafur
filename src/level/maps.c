#include "cimmerian.h"

#define MAP_SIZE_X 24
#define MAP_SIZE_Y 24
#define PLAYER_POS_X 22
#define PLAYER_POS_Y 12

static t_map	*create_map(void);
static void		free_map(t_map **map);
static t_vec2	get_cardinal_dir(char c);

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

int	initialize_maps(void)
{
	g_man.map = create_map();
	return (!!g_man.map);
}

void	free_maps(void)
{
	free_map(&g_man.map);
	return ;
}

static t_map	*create_map(void)
{
	int		i;
	t_map	*map;

	map = calloc(1, sizeof(t_map));
	if (!map)
		return (0);
	map->size.x = MAP_SIZE_X;
	map->size.y = MAP_SIZE_Y;
	map->start_pos.x = PLAYER_POS_X;
	map->start_pos.y = PLAYER_POS_Y;
	map->start_dir = get_cardinal_dir('N');
	map->dof = 0;
	map->fog_width = get_fog_width(map->dof);
	map->fog_color = get_color_rgba(17, 15, 35, 255);
	set_ivec2(&map->minimap_offset, 563, 7);
	set_ivec2(&map->minimap_center, 599, 43);
	map->minimap_radius = 36;
	map->minimap_zoom = 9;
	map->minimap_cell_amount = map->minimap_radius / map->minimap_zoom * 2;
	map->cells = 0;
	map->background = 0;
	map->skybox = compose_skybox(get_sprite("src_skybox"), map->fog_color);
	if (!map->skybox)
	{
		free_map(&map);
		return (0);
	}
	map->background = compose_background(map);
	/*
		`compose_background` is about creating an image from scratch. It's just 
		a buffer. And then, `update_background` is called and it copies the 
		skybox data. So, basically, there's only one frame for the background, 
		because it can just copy the current skybox cycle frame.

		I wonder if I shouldn't just have a t_png object for this one, since it 
		would be a bother to add an image to the general list.

		EDIT: Keep in mind that currently the background is only updated when 
		the fog width changes. It needs to be updated every frame in case the 
		skybox has an animation.
	*/
	if (!map->background)
	{
		free_map(&map);
		return (0);
	}
	map->cells = malloc(map->size.x * map->size.y * sizeof(t_cell));
	if (!map->cells)
	{
		free_map(&map);
		return (0);
	}
	t_spr *spr;
	i = 0;
	while (i < map->size.x * map->size.y)
	{
		map->cells[i].is_obstacle = map_walls[i] > 0;
		map->cells[i].is_door = map_walls[i] == 5;
		map->cells[i].is_goal = map_walls[i] == 6;
		map->cells[i].is_indoors = map_buildings[i];
		map->cells[i].height = !map->cells[i].is_obstacle ? 0 : 1.0;
		if (map_walls[i] == 6)
			map->cells[i].height = 3.0;
		map->cells[i].tex_floor = map_buildings[i] ? get_sprite("tex_floor_indoors") : get_sprite("tex_floor");
		map->cells[i].tex_ceiling = map_buildings[i] ? get_sprite("tex_ceiling") : 0;
		spr = 0;
		if (map_walls[i] == 1)
			spr = get_sprite("tex_wall_01");
		else if (map_walls[i] == 2)
			spr = get_sprite("tex_wall_02");
		else if (map_walls[i] == 3)
			spr = get_sprite("tex_wall_03");
		else if (map_walls[i] == 4)
			spr = get_sprite("tex_wall_04");
		else if (map_walls[i] == 5)
			spr = get_sprite("tex_doors");
		else if (map_walls[i] == 6)
			spr = get_sprite("solid_color");
		map->cells[i].tex_north = spr;
		map->cells[i].tex_east = spr;
		map->cells[i].tex_south = spr;
		map->cells[i].tex_west = spr;
		++i;
	}
	map->objects = malloc(NBR_OBJ * sizeof(t_obj));
	if (!map->objects)
	{
		free_map(&map);
		return (0);
	}
	spr = get_sprite("chalice");
	i = 0;
	while (i < 8)
	{
		map->objects[i].spr = spr;
		++i;
	}
	set_vec2(&map->objects[0].pos,  20.5, 11.5);
	set_vec2(&map->objects[1].pos,  18.5,  4.5);
	set_vec2(&map->objects[2].pos,  10.0,  4.5);
	set_vec2(&map->objects[3].pos,  10.0, 12.5);
	set_vec2(&map->objects[4].pos,   3.5,  6.5);
	set_vec2(&map->objects[5].pos,   3.5, 20.5);
	set_vec2(&map->objects[6].pos,   3.5, 14.5);
	set_vec2(&map->objects[7].pos,  14.5, 20.5);
	spr = get_sprite("pillar");
	while (i < 12)
	{
		map->objects[i].spr = spr;
		++i;
	}
	set_vec2(&map->objects[8].pos,  18.5, 10.5);
	set_vec2(&map->objects[9].pos,  18.5, 11.5);
	set_vec2(&map->objects[10].pos, 18.5, 12.5);
	set_vec2(&map->objects[11].pos,  8.5,  7.0);
	spr = get_sprite("barrel");
	while (i < 20)
	{
		map->objects[i].spr = spr;
		++i;
	}
	set_vec2(&map->objects[12].pos, 21.5,  1.5);
	set_vec2(&map->objects[13].pos, 15.5,  1.5);
	set_vec2(&map->objects[14].pos, 16.0,  1.8);
	set_vec2(&map->objects[15].pos, 16.2,  1.2);
	set_vec2(&map->objects[16].pos,  3.5,  2.5);
	set_vec2(&map->objects[17].pos,  9.5, 15.5);
	set_vec2(&map->objects[18].pos, 10.0, 15.1);
	set_vec2(&map->objects[19].pos, 10.5, 15.8);
	return (map);
}

static void	free_map(t_map **map)
{
	free((*map)->cells);
	free_sprite((*map)->skybox);
	free_image((*map)->background);
	free((*map)->objects);
	free(*map);
	*map = 0;
	return ;
}

static t_vec2	get_cardinal_dir(char c)
{
	t_vec2	dir;

	dir.x = 0;
	dir.y = 0;
	if (c >= 'a' && c <= 'z')
		c -= 'a' - 'A';
	if (c == 'N')
		dir.y = -1;
	else if (c == 'S')
		dir.y = 1;
	else if (c == 'W')
		dir.x = -1;
	else if (c == 'E')
		dir.x = 1;
	return (dir);
}
