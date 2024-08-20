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
/*
#define MAP_WIDTH 24
#define MAP_HEIGHT 24

int world_map[MAP_HEIGHT][MAP_WIDTH] =
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

man.player.pos.x = 22;
man.player.pos.y = 12;
*/

#define MAP_WIDTH 8
#define MAP_HEIGHT 8
int world_map[MAP_HEIGHT][MAP_WIDTH] =
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

static void raycasting(void)
{
    for (int x = 0; x < SCREEN_WIDTH; ++x)
    {
        //calculate ray position and direction
        double camera_x = 2 * x / (double)SCREEN_WIDTH - 1; //x-coordinate in camera space
        t_vec2 ray_dir;
        ray_dir.x = man.player.dir.x + man.player.plane.x * camera_x;
        ray_dir.y = man.player.dir.y + man.player.plane.y * camera_x;
        //which box of the map we're in
        t_ivec2 map;
        map.x = (int)man.player.pos.x;
        map.y = (int)man.player.pos.y;

        //length of ray from current position to next x or y-side
        t_vec2 side_dist;

        //length of ray from one x or y-side to next x or y-side
        //these are derived as:
        //delta_dist.x = sqrt(1 + (ray_dir.y * ray_dir.y) / (ray_dir.x * ray_dir.x))
        //delta_dist.y = sqrt(1 + (ray_dir.x * ray_dir.x) / (ray_dir.y * ray_dir.y))
        //which can be simplified to abs(|ray_dir| / ray_dir.x) and abs(|ray_dir| / ray_dir.y)
        //where |ray_dir| is the length of the vector (ray_dir.x, ray_dir.y). Its length,
        //unlike (man.player.dir.x, man.player.dir.y) is not 1, however this does not matter, only the
        //ratio between delta_dist.x and delta_dist.y matters, due to the way the DDA
        //stepping further below works. So the values can be computed as below.
        // Division through zero is prevented, even though technically that's not
        // needed in C++ with IEEE 754 floating point values.
        t_vec2 delta_dist;
        delta_dist.x = (ray_dir.x == 0) ? 1e30 : f_abs(1 / ray_dir.x);
        delta_dist.y = (ray_dir.y == 0) ? 1e30 : f_abs(1 / ray_dir.y);

        double perp_wall_dist;

        //what direction to step in x or y-direction (either +1 or -1)
        t_ivec2 step;

        int hit = 0; //was there a wall hit?
        int side; //was a NS or a EW wall hit?
                  //calculate step and initial sideDist
        if (ray_dir.x < 0)
        {
            step.x = -1;
            side_dist.x = (man.player.pos.x - map.x) * delta_dist.x;
        }
        else
        {
            step.x = 1;
            side_dist.x = (map.x + 1.0 - man.player.pos.x) * delta_dist.x;
        }
        if (ray_dir.y < 0)
        {
            step.y = -1;
            side_dist.y = (man.player.pos.y - map.y) * delta_dist.y;
        }
        else
        {
            step.y = 1;
            side_dist.y = (map.y + 1.0 - man.player.pos.y) * delta_dist.y;
        }
        //perform DDA
        while (hit == 0)
        {
            //jump to next map square, either in x-direction, or in y-direction
            if (side_dist.x < side_dist.y)
            {
                side_dist.x += delta_dist.x;
                map.x += step.x;
                side = 0;
            }
            else
            {
                side_dist.y += delta_dist.y;
                map.y += step.y;
                side = 1;
            }
            //Check if ray has hit a wall
            if (world_map[map.y][map.x] > 0)
                hit = 1;
        }
        //Calculate distance projected on camera direction. This is the shortest distance from the point where the wall is
        //hit to the camera plane. Euclidean to center camera point would give fisheye effect!
        //This can be computed as (map.x - man.player.pos.x + (1 - step.x) / 2) / ray_dir.x for side == 0, or same formula with Y
        //for size == 1, but can be simplified to the code below thanks to how side_dist and delta_dist are computed:
        //because they were left scaled to |ray_dir|. side_dist is the entire length of the ray above after the multiple
        //steps, but we subtract delta_dist once because one step more into the wall was taken above.
        if (side == 0)
            perp_wall_dist = (side_dist.x - delta_dist.x);
        else
            perp_wall_dist = (side_dist.y - delta_dist.y);

        //Calculate height of line to draw on screen
        int line_height = (int)(SCREEN_HEIGHT / perp_wall_dist);

        //calculate lowest and highest pixel to fill in current stripe
        int draw_start = -line_height / 2 + SCREEN_HEIGHT / 2;
        if (draw_start < 0)
            draw_start = 0;
        int draw_end = line_height / 2 + SCREEN_HEIGHT / 2;
        if (draw_end >= SCREEN_HEIGHT)
            draw_end = SCREEN_HEIGHT - 1;

        //choose wall color
        t_color color;
        switch (world_map[map.y][map.x])
        {
            case 1:  color = get_color_rgba(255,   0,   0, 255); break; //red
            case 2:  color = get_color_rgba(  0, 255,   0, 255); break; //green
            case 3:  color = get_color_rgba(  0,   0, 255, 255); break; //blue
            case 4:  color = get_color_rgba(255, 255, 255, 255); break; //white
            default: color = get_color_rgba(255, 255,   0, 255); break; //yellow
        }

        //give x and y sides different brightness
        if (side == 1)
        {
            color.r = color.r / 2;
            color.g = color.g / 2;
            color.b = color.b / 2;
        }

        //draw the pixels of the stripe as a vertical line
        t_vert v1;
        t_vert v2;
        v1.coord.x = x;
        v1.coord.y = draw_start;
        v1.color = color;
        v2.coord.x = x;
        v2.coord.y = draw_end;
        v2.color = color;
        draw_line(man.tex[man.curr_tex], v1, v2);
    }

    //the constant value is in squares/second
    double move_speed;
    //the constant value is in radians/second
    double rot_speed;

    /* Move along the forward axis */
    move_speed = man.movement_action[2] * 2.0 * man.delta_time;
    if (!world_map[(int)man.player.pos.y][(int)(man.player.pos.x + man.player.dir.x * move_speed)])
        man.player.pos.x += man.player.dir.x * move_speed;
    if (!world_map[(int)(man.player.pos.y + man.player.dir.y * move_speed)][(int)man.player.pos.x])
        man.player.pos.y += man.player.dir.y * move_speed;
    /* Move along the lateral axis */
    move_speed = man.movement_action[0] * 2.0 * man.delta_time;
    if (!world_map[(int)man.player.pos.y][(int)(man.player.pos.x + -man.player.dir.y * move_speed)])
        man.player.pos.x += -man.player.dir.y * move_speed;
    if (!world_map[(int)(man.player.pos.y + man.player.dir.x * move_speed)][(int)man.player.pos.x])
        man.player.pos.y += man.player.dir.x * move_speed;

    //both camera direction and camera plane must be rotated
    t_vec2 old_dir;
    t_vec2 old_plane;
    rot_speed = man.rotation_action * RAD_45 * man.delta_time;
    old_dir.x = man.player.dir.x;
    man.player.dir.x = man.player.dir.x * f_cos(rot_speed) - man.player.dir.y * f_sin(rot_speed);
    man.player.dir.y = old_dir.x * f_sin(rot_speed) + man.player.dir.y * f_cos(rot_speed);
    old_plane.x = man.player.plane.x;
    man.player.plane.x = man.player.plane.x * f_cos(rot_speed) - man.player.plane.y * f_sin(rot_speed);
    man.player.plane.y = old_plane.x * f_sin(rot_speed) + man.player.plane.y * f_cos(rot_speed);
    return;
}
