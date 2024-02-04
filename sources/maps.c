#include "../headers/maps.h"
#include "../headers/maths.h"
#include "../headers/draw.h"

Map* map_test = {0};
Player player = {0};
static Vector offset = {0};

static void draw_map(const Map* m);
static void draw_player(const Map* m);

/* 1 is a wall, 0 is an empty space */
static int map_default[] = 
{
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 1, 0, 0, 0, 0, 1,
    1, 0, 1, 0, 0, 0, 0, 1,
    1, 0, 1, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 1, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
};

Map* create_map(void)
{
    int i;
    Map* map = malloc(sizeof(Map));
    if (!map)
        return 0;

    map->width = 8;
    map->height = 8;
    map->cell = 40;

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

void draw_minimap(const Map* map)
{
    draw_map(map);
    draw_player(map);
    return;
}

void free_map(Map** map)
{
    free((*map)->data);
    free(*map);

    /* Nullify the reference to prevent a double free */
    *map = 0;
    return;
}

static void draw_map(const Map* m)
{
    int x, y;
    Vertex v;

    for (y = 0; y < m->height; ++y)
    {
        for (x = 0; x < m->width; ++x)
        {
            if (m->data[(m->height-1-y) * m->width + x] == 1)
                v.color = 0xFF;
            else
                v.color = 0;

            v.coords.x = offset.x + x * m->cell;
            v.coords.y = offset.y + y * m->cell;
            /* Remove 1 pixel in order to see grid lines */
            draw_rectangle(TEX_MAIN, 1, v, m->cell-1, m->cell-1);
        }
    }
    return;
}

static void draw_player(const Map* m)
{
    const int player_size = MAX(1, m->cell/5);
    const int forward_vector_length = MAX(2, m->cell/8);
    Vertex pos, rot_end;

    /* Position */
    pos.coords.x = offset.x + player.pos.x - player_size/2;
    pos.coords.y = offset.y + player.pos.y - player_size/2;
    pos.color = get_color_from_rgb(MAX_RED, MAX_GREEN, 0);
    draw_rectangle(TEX_MAIN, 1, pos, player_size, player_size);

    /* Rotation: Line to represent where the player is looking */
    pos.coords.x = offset.x + player.pos.x;
    pos.coords.y = offset.y + player.pos.y;
    rot_end.coords.x = pos.coords.x + player.delta.x * forward_vector_length;
    rot_end.coords.y = pos.coords.y + player.delta.y * forward_vector_length;
    rot_end.color = pos.color;
    draw_line(TEX_MAIN, pos, rot_end);
    return;
}

