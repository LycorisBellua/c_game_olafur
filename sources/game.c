#include "../headers/game.h"
#include "../headers/textures.h"
#include "../headers/input.h"
#include "../headers/time.h"
#include "../headers/draw.h"

/*
    "mapX" is the amount of squares on the X axis
    "mapY" is the amount of squares on the Y axis
    It amounts to a 8x8 grid

    "mapS" is the size of a square in pixels
    He picked 64 for mapS, but I changed it to 45 because between my width 
    and height, my height is smaller and it's 360, and 360/8=45.
*/
int mapX = 8, mapY = 8, mapS = 45;

/* 1 is a wall, 0 is an empty space */
int map[] = 
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

float px, py;
const int player_size = 8;
/*
    "px" is "player X position"
    "py" is "player Y position"
    These position values may or may not match the 2D map, 
    one thing is certain is that they are the pixels on the screen. 
    Well, these floats turned into ints are the pixels.
*/

static void draw_map_2d(void);
static void draw_player(void);

void draw_game(void)
{
    draw_map_2d();
    draw_player();
    return;
}

void reset_global_coordinates(void)
{
    px = get_coord_x(TEX_MAIN, 0.5f) - player_size/2;
    py = get_coord_y(TEX_MAIN, 0.5f) - player_size/2;
    return;
}

void update_global_coordinates(void)
{
    /*
        For me, UP is an addition and DOWN a substraction, but it's inverted for him:
       - W -> py -= 5; //It's supposed to be UP, but for some reason he says it's DOWN
       - A -> px -= 5; //Yep, that's LEFT alright
       - S -> py += 5; //It's supposed to be DOWN, but for some reason he says it's UP
       - D -> px += 5; //Yep, that's RIGHT alright
    */
    const float speed = 50.0f;
    px += movement_action[0] * speed * delta_time;
    py += movement_action[2] * speed * delta_time;
    return;
}

static void draw_map_2d(void)
{
    /* This offset is to center the drawing */
    const int x_offset = 140;
    int x, y;
    Vertex v;

    for (y = 0; y < mapY; ++y)
    {
        for (x = 0; x < mapX; ++x)
        {
            if (map[(mapY-1-y) * mapX + x] == 1)
            {
                /* Set color to white for wall */
                v.color = 0xFF;
            }
            else
            {
                /* Set color to black for empty space */
                v.color = 0;
            }

            v.coords.x = x_offset + x * mapS;
            v.coords.y = y * mapS;
            /* Remove 1 pixel in order to see grid lines */
            draw_rectangle(TEX_MAIN, 1, v, mapS-1, mapS-1);
        }
    }
    return;
}

static void draw_player(void)
{
    Vertex pos;
    pos.coords.x = px;
    pos.coords.y = py;
    /* Yellow (1, 1, 0): */
    set_color_from_rgb(&pos.color, MAX_RED, MAX_GREEN, 0);
    draw_rectangle(TEX_MAIN, 1, pos, player_size, player_size);
    return;
}

