#include "cimmerian.h"

static void draw_gradient(t_tex* t);
static void raycasting(void);

void draw_game(void)
{
    draw_gradient(man.tex[man.curr_tex]);
    raycasting();
    return;
}

/*
    Horizontal gradient from a dark blue/purple color at the top to a dark 
    greyish brown at the bottom
*/
static void draw_gradient(t_tex* t)
{
    t_vert v;
    t_color top;
    t_color bottom;
    float factor;

    top = get_color_rgba(64, 0, 64, 255);
    bottom = get_color_rgba(58, 38, 42, 255);
    v.coord.y = 0;
    while (v.coord.y < t->size.y)
    {
        v.coord.x = 0;
        while (v.coord.x < t->size.x)
        {
            factor = (float)v.coord.y / (t->size.y - 1);
            v.color.r = top.r + factor * (bottom.r - top.r);
            v.color.g = top.g + factor * (bottom.g - top.g);
            v.color.b = top.b + factor * (bottom.b - top.b);
            v.color.a = 255;
            draw_point(t, v.color, v.coord.x, v.coord.y);
            ++v.coord.x;
        }
        ++v.coord.y;
    }
}

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 360

int map[MAP_HEIGHT][MAP_WIDTH] =
{
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
    {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
    {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};
/*
#define MAP_WIDTH 24
#define MAP_HEIGHT 24
man.player.pos.x = 22;
man.player.pos.y = 12;
*/

/*
int map[MAP_HEIGHT][MAP_WIDTH] =
{
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 1, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1}
};
*/
/*
#define MAP_WIDTH 8
#define MAP_HEIGHT 8
man.player.pos.x = 4;
man.player.pos.y = 4;
*/

static void raycasting(void)
{
    for (int x = 0; x < SCREEN_WIDTH; ++x)
    {
        //calculate ray position and direction
        //camera_x is x-coordinate in camera space
        double camera_x = 2 * x / (double)SCREEN_WIDTH - 1;
        t_vec2 ray_dir;
        ray_dir.x = man.player.dir.x + man.player.plane.x * camera_x;
        ray_dir.y = man.player.dir.y + man.player.plane.y * camera_x;
        t_ivec2 map_index;
        map_index.x = (int)man.player.pos.x;
        map_index.y = (int)man.player.pos.y;

        //length of ray from current position to next x or y-side
        t_vec2 side_dist;

        //length of ray from one x or y-side to next x or y-side
        t_vec2 delta_dist;
        delta_dist.x = (ray_dir.x == 0) ? 1e30 : f_abs(1 / ray_dir.x);
        delta_dist.y = (ray_dir.y == 0) ? 1e30 : f_abs(1 / ray_dir.y);
        /*
            These are derived as:
                delta_dist.x = sqrt(1 + pow(ray_dir.y, 2) / pow(ray_dir.x, 2))
                delta_dist.y = sqrt(1 + pow(ray_dir.x, 2) / pow(ray_dir.y, 2))
            which can be simplified to:
                abs(|ray_dir| / ray_dir.x)
                abs(|ray_dir| / ray_dir.y)
            where |ray_dir| is the length of the vector (ray_dir.x, ray_dir.y).
            Its length, unlike (dir.x, dir.y) is not 1, however this does not 
            matter, only the ratio between delta_dist.x and delta_dist.y 
            matters, due to the way the DDA stepping further below works. So 
            the values can be computed as below. Division by zero is prevented.
        */

        //what direction to step in x or y-direction (either +1 or -1)
        t_ivec2 step;

        //was there a wall hit?
        int hit = 0;
        //was a NS or a EW wall hit?
        int side;
        //calculate step and initial side_dist
        if (ray_dir.x < 0)
        {
            step.x = -1;
            side_dist.x = (man.player.pos.x - map_index.x) * delta_dist.x;
        }
        else
        {
            step.x = 1;
            side_dist.x = (map_index.x + 1.0 - man.player.pos.x) * delta_dist.x;
        }
        if (ray_dir.y < 0)
        {
            step.y = -1;
            side_dist.y = (man.player.pos.y - map_index.y) * delta_dist.y;
        }
        else
        {
            step.y = 1;
            side_dist.y = (map_index.y + 1.0 - man.player.pos.y) * delta_dist.y;
        }
        //perform DDA
        while (hit == 0)
        {
            //jump to next map square, either in x-direction, or in y-direction
            if (side_dist.x < side_dist.y)
            {
                side_dist.x += delta_dist.x;
                map_index.x += step.x;
                side = 0;
            }
            else
            {
                side_dist.y += delta_dist.y;
                map_index.y += step.y;
                side = 1;
            }
            //check if ray has hit a wall
            if (map[map_index.y][map_index.x] > 0)
                hit = 1;
        }
        //calculate distance projected on camera direction
        double perp_wall_dist;
        if (side == 0)
            perp_wall_dist = (side_dist.x - delta_dist.x);
        else
            perp_wall_dist = (side_dist.y - delta_dist.y);
        /*
            This is the shortest distance from the point where the wall is hit 
            to the camera plane. Euclidean to center camera point would give 
            fisheye effect! This can be computed as (map_index.x - pos.x + 
            (1 - step.x) / 2) / ray_dir.x for side == 0, or same formula with Y 
            for size == 1, but can be simplified to the code below thanks to 
            how side_dist and delta_dist are computed: because they were left 
            scaled to |ray_dir|. side_dist is the entire length of the ray 
            above after the multiple steps, but we subtract delta_dist once 
            because one step more into the wall was taken above.
        */

        //calculate height of line to draw on screen
        int line_height = (int)(SCREEN_HEIGHT / perp_wall_dist);

        //calculate lowest and highest pixel to fill in current stripe
        t_vert v1;
        t_vert v2;
        v1.coord.x = x;
        v1.coord.y = -line_height / 2 + SCREEN_HEIGHT / 2;
        if (v1.coord.y < 0)
            v1.coord.y = 0;
        v2.coord.x = x;
        v2.coord.y = line_height / 2 + SCREEN_HEIGHT / 2;
        if (v2.coord.y >= SCREEN_HEIGHT)
            v2.coord.y = SCREEN_HEIGHT - 1;

        //choose wall color
        t_color color;
        if (map[map_index.y][map_index.x] == 1)
            color = get_color_rgba( 93,  42,  98, 255); //purple
        else if (map[map_index.y][map_index.x] == 2)
            color = get_color_rgba( 78, 120,  94, 255); //green
        else if (map[map_index.y][map_index.x] == 3)
            color = get_color_rgba( 83, 120, 156, 255); //blue
        else if (map[map_index.y][map_index.x] == 4)
            color = get_color_rgba(155, 114,  44, 255); //yellow
        else
            color = get_color_rgba(255, 255, 255, 255); //white
        //give x and y sides different brightness
        if (side == 1)
        {
            color.r -= color.r / 5;
            color.g -= color.g / 5;
            color.b -= color.b / 5;
        }
        v1.color = color;
        v2.color = color;
        draw_line(man.tex[man.curr_tex], v1, v2);
    }
    return;
}
