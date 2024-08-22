#include "cimmerian.h"

#define MINIMAP_FACTOR_MIN 1
#define MINIMAP_FACTOR_MAX 5

/*
#define MAP_SIZE_X 8
#define MAP_SIZE_Y 8
#define PLAYER_POS_X 4
#define PLAYER_POS_Y 4
*/
/*
#define MAP_SIZE_X 16
#define MAP_SIZE_Y 16
#define PLAYER_POS_X 8
#define PLAYER_POS_Y 8
*/
#define MAP_SIZE_X 24
#define MAP_SIZE_Y 24
#define PLAYER_POS_X 22
#define PLAYER_POS_Y 12

static int minimap_factor = MINIMAP_FACTOR_MAX;
static t_ivec2 display_offset = {0};

static t_map* create_map(void);
static void free_map(t_map** map);
static t_vec2 get_cardinal_dir(char c);

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

    /*
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
    */

    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1,
    1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1,
    1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
};

void initialize_maps(void)
{
    man.map = create_map();
    set_minimap_display(0);
    return;
}

void set_minimap_display(int remove_from_factor)
{
    int cell;
    t_tex* t;

    if (!remove_from_factor)
        minimap_factor = MINIMAP_FACTOR_MAX;
    else
        minimap_factor = clamp(minimap_factor - remove_from_factor, 
            MINIMAP_FACTOR_MIN, MINIMAP_FACTOR_MAX);
    t = man.tex[man.curr_tex];
    cell = MAP_CELL_LEN/minimap_factor;
    display_offset.x = t->size.x - cell * MAX_CELL_AMOUNT - 50;
    display_offset.y = t->size.y - cell * MAX_CELL_AMOUNT - 50;
    return;
}

/*
void draw_minimap(t_map* map)
{
    draw_map(map);
    draw_player();
    return;
}
*/

void free_maps(void)
{
    free_map(&man.map);
    return;
}

static t_map* create_map(void)
{
    int i;
    t_map* map;

    map = malloc(sizeof(t_map));
    if (!map)
        return 0;
    map->size.x = MAP_SIZE_X;
    map->size.y = MAP_SIZE_Y;
    map->start_pos.x = PLAYER_POS_X;
    map->start_pos.y = PLAYER_POS_Y;
    map->start_dir = get_cardinal_dir('N');
    map->dof = 8;
    map->fog_width = get_fog_width(map->dof);
    map->fog_color = get_color_rgba(0, 0, 0, 255);
    map->data = malloc(map->size.x * map->size.y * sizeof(int));
    if (!map->data)
    {
        free(map);
        return 0;
    }
    i = 0;
    while (i < map->size.x * map->size.y)
    {
        map->data[i] = map_default[i];
        ++i;
    }
    return map;
}

static void free_map(t_map** map)
{
    free((*map)->data);
    free(*map);

    /* Nullify the reference to prevent a double free */
    *map = 0;
    return;
}

static t_vec2 get_cardinal_dir(char c)
{
    t_vec2 dir;

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
    return dir;
}

/*
static void draw_map(t_map* m)
{
    int max_len_cell;
    int max_len_map;
    t_ivec2 len;
    t_ivec2 rect_size;
    t_vec2 pos;
    int x;
    int y;
    t_vert v;

    max_len_cell = MAP_CELL_LEN/minimap_factor;
    max_len_map = max_len_cell * MAX_CELL_AMOUNT;
    v.coord.x = display_offset.x;
    v.coord.y = display_offset.y;
    v.color = get_color_rgba(255, 0, 0, 255);
    rect_size.x = max_len_map;
    rect_size.y = max_len_map;
    draw_rectangle(man.tex[man.curr_tex], v, rect_size);

    //SMALLEST MAP
    //- MAX LEN FOR THE MAP: 96
    //- DISPLAY OFFSET (544, 264)

    len.x = max_len_cell;
    len.y = max_len_cell;
    pos.x = man.player.pos.x/MAP_CELL_LEN - MAX_CELL_AMOUNT/2;
    pos.y = man.player.pos.y/MAP_CELL_LEN - MAX_CELL_AMOUNT/2;

    v.coord.y = display_offset.y;
    y = pos.y;
    while (y < m->size.y)
    {
        if (y == (int)pos.y)
            len.y = max_len_cell - (pos.y - (int)pos.y) * max_len_cell;

        if (y < 0)
        {
            v.coord.y += len.y;
            len.y = max_len_cell;
            continue;
        }
        else if (y == 0 && len.y > max_len_cell)
        {
            v.coord.y += len.y - max_len_cell;
            len.y = max_len_cell;
        }

        v.coord.x = display_offset.x;
        x = pos.x;
        while (x < m->size.x)
        {
            if (x == (int)pos.x)
                len.x = max_len_cell - (pos.x - (int)pos.x) * max_len_cell;

            if (x < 0)
            {
                v.coord.x += len.x;
                len.x = max_len_cell;
                continue;
            }
            else if (x == 0 && len.x > max_len_cell)
            {
                v.coord.x += len.x - max_len_cell;
                len.x = max_len_cell;
            }

            if (m->data[y * m->size.x + x] == 1)
                v.color = get_color_rgba(255, 255, 255, 255);
            else
                v.color = get_color_rgba(0, 0, 0, 0);

            // Remove 1 pixel in order to see grid lines
            rect_size.x = len.x - 1;
            rect_size.y = len.y - 1;
            draw_rectangle_full(man.tex[man.curr_tex], v, rect_size);
            v.coord.x += len.x;
            len.x = max_len_cell;
            if (v.coord.x > display_offset.x + max_len_map)
                len.x -= v.coord.x - (display_offset.x + max_len_map);

            ++x;
        }

        v.coord.y += len.y;
        len.y = max_len_cell;
        ++y;
    }
    return;
}

static void draw_player(void)
{
    int player_size;
    int forward_vector_len;
    double map_center;
    double offset;
    t_vert pos;
    t_vert end;
    t_ivec2 rect_size;

    player_size = max(1, MAP_CELL_LEN/4.0/minimap_factor);
    forward_vector_len = max(2, MAP_CELL_LEN/8.0/minimap_factor);
    map_center = MAX_CELL_AMOUNT/2 * MAP_CELL_LEN / minimap_factor;
    offset = get_minimap_factor_offset(minimap_factor);

    pos.coord.x = map_center + display_offset.x - offset;
    pos.coord.y = map_center + display_offset.y - offset;
    pos.color = get_color_rgba(255, 255, 0, 255);

    end.coord.x = pos.coord.x + man.player.delta.x * forward_vector_len;
    end.coord.y = pos.coord.y + man.player.delta.y * forward_vector_len;
    end.color = pos.color;

    // Forward vector
    draw_line(man.tex[man.curr_tex], pos, end);

    pos.coord.x -= player_size/2;
    pos.coord.y -= player_size/2;
    pos.color = get_color_rgba(255, 0, 0, 255);

    // Position
    rect_size.x = player_size;
    rect_size.y = player_size;
    draw_rectangle_full(man.tex[man.curr_tex], pos, rect_size);
    return;
}

static double get_minimap_factor_offset(int factor)
{
    double offset;
    double quotient;
    double modulus;

    offset = 0;
    if (factor > 1 && factor % 2)
    {
        quotient = factor/2.0;
        modulus = factor % 2;
        offset = quotient + modulus + (quotient-modulus) - 1;
    }
    return offset;
}
*/
