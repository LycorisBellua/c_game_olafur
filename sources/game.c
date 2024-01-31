#include "../headers/game.h"
#include "../headers/textures.h"
#include "../headers/input.h"
#include "../headers/time.h"
#include "../headers/maths.h"
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

float px, py, pdx, pdy, pa;
const int player_size = 8;
/*
    "px" is "player X position"
    "py" is "player Y position"
    These position values may or may not match the 2D map, 
    one thing is certain is that they are the pixels on the screen. 
    Well, these floats turned into ints are the pixels.

    "pdx" is "player delta X"
    "pdy" is "player delta Y"
    "pa" is "player angle"
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
    pdx = f_cos(pa)*5;
    pdy = f_sin(pa)*5;
    return;
}

void update_global_coordinates(void)
{
    const float speed = 20.0f;
    const float min_rad = 0.0f;
    const float max_rad = 2*PI;

    /*
        Angles are in radians instead of degrees. While degrees go from 
        0 to 360, radians go from 0 to 2PI which amounts to 6.28. This 
        is the reason why such a small value (0.1f) is used here.

        The values returned by the sine and cosine operations are very 
        small, so multiply them by 5.
    */

    pa += rotation_action * 0.1f * speed * delta_time;
    if (rotation_action)
    {
        /* Turning left */
        if (pa < min_rad && rotation_action < 0)
            pa += max_rad;
        /* Turning right */
        else if (pa > max_rad && rotation_action > 0)
            pa -= max_rad;

        pdx = f_cos(pa)*5;
        pdy = f_sin(pa)*5;
    }

    /* Local UP and DOWN movement */
    px += movement_action[2] * pdx * speed * delta_time;
    py += movement_action[2] * pdy * speed * delta_time;

    /* Left and right strafes? */
    return;
}

static void draw_map_2d(void)
{
    const int x_offset = (TEX_MAIN->width - mapX*mapS) / 2;
    const int y_offset = (TEX_MAIN->height - mapY*mapS) / 2;
    int x, y;
    Vertex v;

    for (y = 0; y < mapY; ++y)
    {
        for (x = 0; x < mapX; ++x)
        {
            if (map[(mapY-1-y) * mapX + x] == 1)
                v.color = 0xFF;
            else
                v.color = 0;

            v.coords.x = x_offset + x * mapS;
            v.coords.y = y_offset + y * mapS;
            /* Remove 1 pixel in order to see grid lines */
            draw_rectangle(TEX_MAIN, 1, v, mapS-1, mapS-1);
        }
    }
    return;
}

static void draw_player(void)
{
    Vertex pos, rot_end;

    /* Position */
    pos.coords.x = px;
    pos.coords.y = py;
    pos.color = get_color_from_rgb(MAX_RED, MAX_GREEN, 0); /* yellow (1,1,0) */
    draw_rectangle(TEX_MAIN, 1, pos, player_size, player_size);

    /* Rotation: Line to represent where the player is looking */
    pos.coords.x = px + player_size/2;
    pos.coords.y = py + player_size/2;
    rot_end.coords.x = pos.coords.x + pdx*5;
    rot_end.coords.y = pos.coords.y + pdy*5;
    rot_end.color = pos.color;
    draw_line(TEX_MAIN, pos, rot_end);
    return;
}

