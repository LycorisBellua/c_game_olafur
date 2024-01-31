#include "../headers/game.h"
#include "../headers/textures.h"
#include "../headers/input.h"
#include "../headers/time.h"
#include "../headers/draw.h"

float px, py;
/*
    "px" is "player X position"
    "py" is "player Y position"
    These position values may or may not match the 2D map, 
    one thing is certain is that they are the pixels on the screen. 
    Well, these floats turned into ints are the pixels.
*/

static void draw_player(void);

void draw_game(void)
{
    draw_player();
    return;
}

void reset_global_coordinates(void)
{
    /*
        He picked 300 and 300 for the default coordinates. 
        With his window dimensions it's at 29.29% of the window for X and 58.59% for Y.
    */
    px = get_coord_x(TEX_MAIN, 0.3f);
    py = get_coord_y(TEX_MAIN, 0.6f);
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

static void draw_player(void)
{
    Vertex pos;
    pos.coords.x = px;
    pos.coords.y = py;
    /* Yellow (1, 1, 0): */
    set_color_from_rgb(&pos.color, MAX_RED, MAX_GREEN, 0);
    draw_rectangle(TEX_MAIN, 1, pos, 8, 8);
    return;
}

