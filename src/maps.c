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
	map->img_len = 13;
	map->img = malloc(map->img_len * sizeof(t_img *));
	if (!map->img)
	{
		free(map);
		return (0);
	}
	map->skybox = compose_skybox("img/textures/skybox.png", map->fog_color);
	map->img[0] = compose_background(map);
	map->img[1] = load_image_from_file("img/textures/wall_01.png");
	map->img[2] = load_image_from_file("img/textures/wall_02.png");
	map->img[3] = load_image_from_file("img/textures/wall_03.png");
	map->img[4] = load_image_from_file("img/textures/wall_04.png");
	map->img[5] = load_image_from_file("img/textures/doors.png");

	t_ivec2	obelisk_size = { 128, 128 };
	map->img[6] = create_image(get_color_rgba(88, 69, 84, 255 * 0.75), obelisk_size);

	map->img[7] = load_image_from_file("img/textures/floor.png");
	map->img[8] = load_image_from_file("img/textures/floor_indoors.png");
	map->img[9] = load_image_from_file("img/textures/ceiling.png");
	map->img[10] = load_image_from_file("img/sprites/wf_barrel.png");
	map->img[11] = load_image_from_file("img/sprites/wf_pillar.png");
	map->img[12] = load_image_from_file("img/sprites/wf_greenlight.png");
	if (!map->skybox || !map->img[0] || !map->img[1] || !map->img[2]
		|| !map->img[3] || !map->img[4] || !map->img[5] || !map->img[6]
		|| !map->img[7] || !map->img[8] || !map->img[9] || !map->img[10]
		|| !map->img[11] || !map->img[12])
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
		map->cells[i].tex_floor = map_buildings[i] ? map->img[8] : map->img[7];
		map->cells[i].tex_ceiling = map_buildings[i] ? map->img[9] : 0;
		map->cells[i].tex_north = map_walls[i] ? map->img[map_walls[i]] : 0;
		map->cells[i].tex_east = map_walls[i] ? map->img[map_walls[i]] : 0;
		map->cells[i].tex_south = map_walls[i] ? map->img[map_walls[i]] : 0;
		map->cells[i].tex_west = map_walls[i] ? map->img[map_walls[i]] : 0;
		++i;
	}
	return (map);
}

static void	free_map(t_map **map)
{
	int	i;

	free((*map)->cells);
	free_image((*map)->skybox);
	i = 0;
	while (i < (*map)->img_len)
	{
		free_image((*map)->img[i]);
		++i;
	}
	free((*map)->img);
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
