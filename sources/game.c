#include "../headers/game.h"
#include "../headers/maps.h"
#include "../headers/player.h"
#include "../headers/maths.h"
#include "../headers/textures.h"
#include "../headers/draw.h"

#define FOV 60
#define MAX_DEPTH_OF_FIELD 8
#define WALL_HEIGHT 35456
/*
    Logic for WALL_HEIGHT is `MAP_CELL_LEN * dist_proj_plane`.
    Second value is `TEX_MAIN->width / (2.0f * f_tan(DEG2RAD(FOV)/2))`.
    In other words: `WALL_HEIGHT = 64 * 554`.
*/

static const GLubyte color_floor = 102;
static const GLubyte color_ceiling = 103;
static const GLubyte color_vertical_wall = 69;
static const GLubyte color_horizontal_wall = 101;

static void draw_floor_and_ceiling(void);
static int is_player_out_of_bounds(const VectorF pos, const Map* m);
static void raycasting(const Map* m);
static float get_horizontal_distance(const Map* m, const int map_val, 
    const float tan, const float ray_angle);
static float get_vertical_distance(const Map* m, const int map_val, 
    const float tan, const float ray_angle);
static void fix_fisheye_effect(float* distance, const float ray_angle);
static void draw_wall(const GLubyte color, const float distance, const int ray);

void draw_game(void)
{
    draw_floor_and_ceiling();
    if (!is_player_out_of_bounds(player.pos, map_test))
        raycasting(map_test);
    draw_minimap(map_test);
    return;
}

void reset_global_coordinates(void)
{
    set_minimap_display(0);
    reset_player_transform(map_test);
    return;
}

static void draw_floor_and_ceiling(void)
{
    Vertex v;

    v.coords.x = 0;
    v.coords.y = 0;
    v.color = color_floor;
    draw_rectangle(TEX_MAIN, 1, v, TEX_MAIN->width, TEX_MAIN->height*0.5f);

    v.coords.y = TEX_MAIN->height*0.5f;
    v.color = color_ceiling;
    draw_rectangle(TEX_MAIN, 1, v, TEX_MAIN->width, TEX_MAIN->height*0.5f);
    return;
}

static int is_player_out_of_bounds(const VectorF pos, const Map* m)
{
    if (pos.x < 0 || pos.x > m->width * MAP_CELL_LEN)
        return 1;
    else if (pos.y < 0 || pos.y > m->height * MAP_CELL_LEN)
        return 1;
    return 0;
}

static void raycasting(const Map* m)
{
    const float ray_increment = RAD_1/(TEX_MAIN->width/FOV);
    const int max_distance = MAP_CELL_LEN * MAX_DEPTH_OF_FIELD;
    /* `ray_angle = player.angle` for center */
    float ray_angle = clamp_radians(player.angle + FOV/2.0f * RAD_1);
    float tan, horizontal, vertical, distance;
    int ray, color;

    for (ray = TEX_MAIN->width; ray > 0; --ray)
    {
        tan = f_tan(ray_angle);
        horizontal = get_horizontal_distance(m, 1, tan, ray_angle);
        vertical = get_vertical_distance(m, 1, tan, ray_angle);

        if (vertical <= horizontal)
        {
            distance = vertical;
            color = color_vertical_wall;
        }
        else
        {
            distance = horizontal;
            color = color_horizontal_wall;
        }

        if (distance > max_distance)
        {
            distance = max_distance;
            color = 0;
        }
        fix_fisheye_effect(&distance, ray_angle);
        draw_wall(color, distance, ray);

        ray_angle = clamp_radians(ray_angle - ray_increment);
    }
    return;
}

static float get_horizontal_distance(const Map* m, const int map_val, 
    const float tan, const float ray_angle)
{
    const float atan = -1/tan;
    float distance = 1000000;
    int depth_of_field = 0;
    int map_index;
    VectorF hit, offset;
    Vector max;

    /* If ray is looking down */
    if (ray_angle > RAD_180)
    {
        hit.y = (int)(player.pos.y/MAP_CELL_LEN) * MAP_CELL_LEN;
        hit.x = (player.pos.y - hit.y) * atan + player.pos.x;
        offset.y = -MAP_CELL_LEN;
        offset.x = -offset.y*atan;
    }
    /* If ray is looking up */
    else if (ray_angle < RAD_180)
    {
        hit.y = (int)((player.pos.y+MAP_CELL_LEN)/MAP_CELL_LEN) * MAP_CELL_LEN;
        hit.x = (player.pos.y - hit.y) * atan + player.pos.x;
        offset.y = MAP_CELL_LEN;
        offset.x = -offset.y*atan;
    }
    else
    {
        depth_of_field = MAX_DEPTH_OF_FIELD;
    }

    /* From ray to map array index */
    while (depth_of_field < MAX_DEPTH_OF_FIELD)
    {
        max.x = (int)(hit.x/MAP_CELL_LEN);
        max.y = m->height - (int)(hit.y/MAP_CELL_LEN);
        if (ray_angle < RAD_180) --max.y;
        map_index = max.y*m->width+max.x;
        if (map_index >= 0 && map_index < m->width*m->height 
            && m->data[map_index] == map_val)
        {
            distance = get_distance(player.pos, hit);
            depth_of_field = MAX_DEPTH_OF_FIELD;
        }
        else
        {
            hit.x += offset.x;
            hit.y += offset.y;
            ++depth_of_field;
        }
    }
    return distance;
}

static float get_vertical_distance(const Map* m, const int map_val, 
    const float tan, const float ray_angle)
{
    const float ntan = -tan;
    float distance = 1000000;
    int depth_of_field = 0;
    int map_index;
    VectorF hit, offset;
    Vector max;

    /* If ray is looking left */
    if (ray_angle > RAD_90 && ray_angle < RAD_270)
    {
        hit.x = (int)(player.pos.x/MAP_CELL_LEN) * MAP_CELL_LEN;
        hit.y = (player.pos.x - hit.x) * ntan + player.pos.y;
        offset.x = -MAP_CELL_LEN;
        offset.y = -offset.x*ntan;
    }
    /* If ray is looking right */
    else if (ray_angle < RAD_90 || ray_angle > RAD_270)
    {
        hit.x = (int)((player.pos.x+MAP_CELL_LEN)/MAP_CELL_LEN) * MAP_CELL_LEN;
        hit.y = (player.pos.x - hit.x) * ntan + player.pos.y;
        offset.x = MAP_CELL_LEN;
        offset.y = -offset.x*ntan;
    }
    else
    {
        depth_of_field = MAX_DEPTH_OF_FIELD;
    }

    /* From ray to map array index */
    while (depth_of_field < MAX_DEPTH_OF_FIELD)
    {
        max.x = (int)(hit.x/MAP_CELL_LEN);
        max.y = m->height-1 - (int)(hit.y/MAP_CELL_LEN);
        if (ray_angle > RAD_90 && ray_angle < RAD_270) --max.x;
        map_index = max.y*m->width+max.x;
        if (map_index >= 0 && map_index < m->width*m->height 
            && m->data[map_index] == map_val)
        {
            distance = get_distance(player.pos, hit);
            depth_of_field = MAX_DEPTH_OF_FIELD;
        }
        else
        {
            hit.x += offset.x;
            hit.y += offset.y;
            ++depth_of_field;
        }
    }
    return distance;
}

static void fix_fisheye_effect(float* distance, const float ray_angle)
{
    *distance *= f_cos(clamp_radians(player.angle - ray_angle));
    return;
}

static void draw_wall(const GLubyte color, const float distance, const int ray)
{
    const float height = WALL_HEIGHT / distance;
    Vertex v;
    v.coords.x = TEX_MAIN->width - ray;
    v.coords.y = (TEX_MAIN->height - height) / 2;
    v.color = color;
    draw_line_vertical(TEX_MAIN, v, (TEX_MAIN->height + height) / 2);
    return;
}

