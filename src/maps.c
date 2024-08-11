#include "cimmerian.h"

#define MINIMAP_FACTOR_MIN 1
#define MINIMAP_FACTOR_MAX 5

static int minimap_factor = MINIMAP_FACTOR_MAX;
static Vector display_offset = {0};

static Map* create_map(void);
static void free_map(Map** map);
static void draw_map(const Map* m);
static void draw_player(void);
static double get_minimap_factor_offset(const int factor);

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
    man.map = create_map();
    set_minimap_display(0);
    return;
}

void set_minimap_display(const int remove_from_factor)
{
    int cell;
    Texture *t;

    if (!remove_from_factor)
        minimap_factor = MINIMAP_FACTOR_MAX;
    else
        minimap_factor = clamp(minimap_factor - remove_from_factor, 
            MINIMAP_FACTOR_MIN, MINIMAP_FACTOR_MAX);
    t = man.tex[man.curr_tex];
    cell = MAP_CELL_LEN/minimap_factor;
    display_offset.x = t->width - cell * MAX_CELL_AMOUNT - 50;
    display_offset.y = t->height - cell * MAX_CELL_AMOUNT - 50;
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
    free_map(&man.map);
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
    i = 0;
    while (i < map->width * map->height)
    {
        map->data[i] = map_default[i];
        ++i;
    }
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
    const int max_len_cell = MAP_CELL_LEN/minimap_factor;
    const int max_len_map = max_len_cell * MAX_CELL_AMOUNT;
    Vector len;
    Vector rect_size;
    VectorF pos;
    int x, y;
    Vertex v;

    v.coords.x = display_offset.x;
    v.coords.y = display_offset.y;
    v.color = get_color_rgba(255, 0, 0, 255);
    rect_size.x = max_len_map;
    rect_size.y = max_len_map;
    draw_rectangle(man.tex[man.curr_tex], v, rect_size);

    /*
        SMALLEST MAP
        - MAX LEN FOR THE MAP: 96
        - DISPLAY OFFSET (544, 264)
    */

    len.x = max_len_cell;
    len.y = max_len_cell;
    pos.x = man.player.pos.x/MAP_CELL_LEN - MAX_CELL_AMOUNT/2;
    pos.y = man.player.pos.y/MAP_CELL_LEN - MAX_CELL_AMOUNT/2;

    v.coords.y = display_offset.y;
    y = pos.y;
    while (y < m->height)
    {
        if (y == (int)pos.y)
            len.y = max_len_cell - (pos.y - (int)pos.y) * max_len_cell;

        if (y < 0)
        {
            v.coords.y += len.y;
            len.y = max_len_cell;
            continue;
        }
        else if (y == 0 && len.y > max_len_cell)
        {
            v.coords.y += len.y - max_len_cell;
            len.y = max_len_cell;
        }

        v.coords.x = display_offset.x;
        x = pos.x;
        while (x < m->width)
        {
            if (x == (int)pos.x)
                len.x = max_len_cell - (pos.x - (int)pos.x) * max_len_cell;

            if (x < 0)
            {
                v.coords.x += len.x;
                len.x = max_len_cell;
                continue;
            }
            else if (x == 0 && len.x > max_len_cell)
            {
                v.coords.x += len.x - max_len_cell;
                len.x = max_len_cell;
            }

            if (m->data[(m->height-1-y) * m->width + x] == 1)
                v.color = get_color_rgba(255, 255, 255, 255);
            else
                v.color = get_color_rgba(0, 0, 0, 0);

            /* Remove 1 pixel in order to see grid lines */
            rect_size.x = len.x - 1;
            rect_size.y = len.y - 1;
            draw_rectangle_full(man.tex[man.curr_tex], v, rect_size);
            v.coords.x += len.x;
            len.x = max_len_cell;
            if (v.coords.x > display_offset.x + max_len_map)
                len.x -= v.coords.x - (display_offset.x + max_len_map);

            ++x;
        }

        v.coords.y += len.y;
        len.y = max_len_cell;
        ++y;
    }
    return;
}

static void draw_player(void)
{
    const int player_size = max(1, MAP_CELL_LEN/4.0f/minimap_factor);
    const int forward_vector_len = max(2, MAP_CELL_LEN/8.0f/minimap_factor);
    const double map_center = MAX_CELL_AMOUNT/2 * MAP_CELL_LEN / minimap_factor;
    const double offset = get_minimap_factor_offset(minimap_factor);
    Vertex pos, end;
    Vector rect_size;

    pos.coords.x = map_center + display_offset.x - offset;
    pos.coords.y = map_center + display_offset.y - offset;
    pos.color = get_color_rgba(255, 255, 0, 255);

    end.coords.x = pos.coords.x + man.player.delta.x * forward_vector_len;
    end.coords.y = pos.coords.y + man.player.delta.y * forward_vector_len;
    end.color = pos.color;

    /* Forward vector */
    draw_line(man.tex[man.curr_tex], pos, end);

    pos.coords.x -= player_size/2;
    pos.coords.y -= player_size/2;
    pos.color = get_color_rgba(255, 0, 0, 255);

    /* Position */
    rect_size.x = player_size;
    rect_size.y = player_size;
    draw_rectangle_full(man.tex[man.curr_tex], pos, rect_size);
    return;
}

static double get_minimap_factor_offset(const int factor)
{
    double offset = 0;
    double quotient, modulus;

    if (factor > 1 && factor % 2)
    {
        quotient = factor/2.0f;
        modulus = factor % 2;
        offset = quotient + modulus + (quotient-modulus) - 1;
    }
    return offset;
}
