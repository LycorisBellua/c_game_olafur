#include "../headers/maps.h"
#include "../headers/maths.h"
#include "../headers/draw.h"

Map* map_test = {0};
Player player = {0};
int minimap_zoom = MINIMAP_ZOOM_MAX;
const int cell_len = 64;
static Vector display_offset = {0};

static Map* create_map(void);
static void free_map(Map** map);
static void draw_map(const Map* m);
static void draw_player(void);
static float get_minimap_zoom_offset(const int zoom);

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

void initialize_maps(void)
{
    map_test = create_map();
    set_minimap_display(5);
    return;
}

void set_minimap_display(const double zoom_level)
{
    const int cell = cell_len/zoom_level;
    display_offset.x = TEX_MAIN->width - cell * map_test->width;
    display_offset.y = TEX_MAIN->height - cell * map_test->height;
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

    map->width = 8;
    map->height = 8;
    map->start_pos.x = map->width * cell_len / 2 - 1;
    map->start_pos.y = map->height * cell_len / 2 - 1;
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
    const int len = cell_len/minimap_zoom;
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

            v.coords.x = display_offset.x + x * len;
            v.coords.y = display_offset.y + y * len;
            /* Remove 1 pixel in order to see grid lines */
            draw_rectangle(TEX_MAIN, 1, v, len-1, len-1);
        }
    }
    return;
}

static void draw_player(void)
{
    const int player_size = MAX(1, cell_len/4.0f/minimap_zoom);
    const int forward_vector_len = MAX(2, cell_len/8.0f/minimap_zoom);
    const float offset = get_minimap_zoom_offset(minimap_zoom);
    Vertex pos, end;

    pos.coords.x = player.pos.x / minimap_zoom + display_offset.x - offset;
    pos.coords.y = player.pos.y / minimap_zoom + display_offset.y - offset;
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

static float get_minimap_zoom_offset(const int zoom)
{
    float offset = 0;
    float quotient, modulus;

    if (zoom > 1 && zoom % 2)
    {
        quotient = zoom/2.0f;
        modulus = zoom % 2;
        offset = quotient + modulus + (quotient-modulus) - 1;
    }
    return offset;
}

