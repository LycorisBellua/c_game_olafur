#include "../headers/maps.h"
#include "../headers/maths.h"
#include "../headers/draw.h"
#include "../headers/player.h"

#define MINIMAP_FACTOR_MIN 1
#define MINIMAP_FACTOR_MAX 5

Map* map_test = {0};
static int minimap_factor = MINIMAP_FACTOR_MAX;
static Vector display_offset = {0};

static Map* create_map(void);
static void free_map(Map** map);
static void draw_map(const Map* m);
static void draw_player(void);
static float get_minimap_factor_offset(const int factor);

/* 1 is a wall, 0 is an empty space */
static int map_default[] = 
{
    /*
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 1, 0, 0, 0, 0, 1,
    1, 0, 1, 0, 0, 0, 0, 1,
    1, 0, 1, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 1, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    */

    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

void initialize_maps(void)
{
    map_test = create_map();
    set_minimap_display(0);
    return;
}

void set_minimap_display(const int remove_from_factor)
{
    int cell;

    if (!remove_from_factor)
        minimap_factor = MINIMAP_FACTOR_MAX;
    else
        minimap_factor = CLAMP(minimap_factor - remove_from_factor, 
            MINIMAP_FACTOR_MIN, MINIMAP_FACTOR_MAX);

    cell = MAP_CELL_LEN/minimap_factor;
    display_offset.x = TEX_MAIN->width - cell * MAX_CELL_AMOUNT;
    display_offset.y = TEX_MAIN->height - cell * MAX_CELL_AMOUNT;
    return;
}

void draw_minimap(const Map* map)
{
    draw_map(map);
    draw_player();
    return;
}

void free_maps(void)
{
    free_map(&map_test);
    return;
}

static Map* create_map(void)
{
    int i;
    Map* map = malloc(sizeof(Map));
    if (!map)
        return 0;

    map->width = 16;
    map->height = 16;
    map->start_pos.x = map->width * MAP_CELL_LEN / 2 - 1;
    map->start_pos.y = map->height * MAP_CELL_LEN / 2 - 1;
    map->start_angle = RAD_90;

    map->data = malloc(map->width * map->height * sizeof(int));
    if (!map->data)
    {
        free(map);
        return 0;
    }
    for (i = 0; i < map->width * map->height; ++i)
        map->data[i] = map_default[i];

    return map;
}

static void free_map(Map** map)
{
    free((*map)->data);
    free(*map);

    /* Nullify the reference to prevent a double free */
    *map = 0;
    return;
}

static void draw_map(const Map* m)
{
    const int max_len = MAP_CELL_LEN/minimap_factor;
    int len;
    int x, y;
    Vertex v;
    VectorF pos;
    pos.x = player.pos.x/MAP_CELL_LEN - MAX_CELL_AMOUNT/2;
    pos.y = player.pos.y/MAP_CELL_LEN - MAX_CELL_AMOUNT/2;

    /**/len = max_len;

    v.coords.y = display_offset.y;
    for (y = pos.y; y < m->height; ++y)
    {
        if (y < 0)
        {
            v.coords.y += len;
            continue;
        }

        v.coords.x = display_offset.x;
        for (x = pos.x; x < m->width; ++x)
        {
            if (x < 0)
            {
                v.coords.x += len;
                continue;
            }
            else if (m->data[(m->height-1-y) * m->width + x] == 1)
                v.color = 0xFF;
            else
                v.color = 0;
            
            /* Remove 1 pixel in order to see grid lines */
            draw_rectangle(TEX_MAIN, 1, v, len-1, len-1);
            v.coords.x += len;
        }

        v.coords.y += len;
    }
    return;
}

static void draw_player(void)
{
    const int player_size = MAX(1, MAP_CELL_LEN/4.0f/minimap_factor);
    const int forward_vector_len = MAX(2, MAP_CELL_LEN/8.0f/minimap_factor);
    const float map_center = MAX_CELL_AMOUNT/2 * MAP_CELL_LEN / minimap_factor;
    const float offset = get_minimap_factor_offset(minimap_factor);
    Vertex pos, end;

    pos.coords.x = map_center + display_offset.x - offset;
    pos.coords.y = map_center + display_offset.y - offset;
    pos.color = get_color_from_rgb(MAX_RED, MAX_GREEN, 0);

    end.coords.x = pos.coords.x + player.delta.x * forward_vector_len;
    end.coords.y = pos.coords.y + player.delta.y * forward_vector_len;
    end.color = pos.color;

    /* Forward vector */
    draw_line(TEX_MAIN, pos, end);

    pos.coords.x -= player_size/2;
    pos.coords.y -= player_size/2;
    pos.color = get_color_from_rgb(MAX_RED, 0, 0);

    /* Position */
    draw_rectangle(TEX_MAIN, 1, pos, player_size, player_size);
    return;
}

static float get_minimap_factor_offset(const int factor)
{
    float offset = 0;
    float quotient, modulus;

    if (factor > 1 && factor % 2)
    {
        quotient = factor/2.0f;
        modulus = factor % 2;
        offset = quotient + modulus + (quotient-modulus) - 1;
    }
    return offset;
}

