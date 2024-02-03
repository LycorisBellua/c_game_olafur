#include "../headers/game.h"
#include "../headers/textures.h"
#include "../headers/input.h"
#include "../headers/time.h"
#include "../headers/maths.h"
#include "../headers/coords.h"
#include "../headers/draw.h"

/*
    "mapX" is the amount of squares on the X axis
    "mapY" is the amount of squares on the Y axis
    It amounts to a 8x8 grid

    "mapS" is the size of a square in pixels
*/
int mapX = 8, mapY = 8, mapS = 40;
int map_x_offset, map_y_offset;

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

VectorF p;
float pdx, pdy, pa;
const int player_size = 8;
/*
    "px" is "player X position"
    "py" is "player Y position"
    "pdx" is "player delta X"
    "pdy" is "player delta Y"
    "pa" is "player angle"
*/

static void draw_map(void);
static void draw_player(void);
static void draw_floor_and_ceiling(void);
static void raycasting(void);
static float get_safe_angle(const float angle);

void draw_game(void)
{
    draw_map();
    draw_player();
    draw_floor_and_ceiling();
    raycasting();
    return;
}

void reset_global_coordinates(void)
{
    map_x_offset = TEX_MAIN->width*0.01f;
    map_y_offset = (TEX_MAIN->height - mapY*mapS) / 2;
    p.x = mapX*mapS/2;
    p.y = mapY*mapS/2;

    pa = get_safe_angle(RAD_90);
    pdx = f_cos(pa)*5;
    pdy = f_sin(pa)*5;
    return;
}

void update_global_coordinates(void)
{
    const float movement_speed = 10.0f;
    const float rotation_speed = 40.0f;

    pa += rotation_action * RAD_1 * rotation_speed * delta_time;
    if (rotation_action)
    {
        pa = clamp_radians(pa);
        pa = get_safe_angle(pa);

        /*
            The values returned by the sine and cosine operations are 
            very small, so multiply them by 5.
        */
        pdx = f_cos(pa)*5;
        pdy = f_sin(pa)*5;
    }

    /* Movement along the forward axis */
    p.x += movement_action[2] * pdx * movement_speed * delta_time;
    p.y += movement_action[2] * pdy * movement_speed * delta_time;

    /* Movement along the lateral axis */
    p.x += movement_action[0] * pdy * movement_speed * delta_time;
    p.y += movement_action[0] * -pdx * movement_speed * delta_time;
    return;
}

static void draw_map(void)
{
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

            v.coords.x = map_x_offset + x * mapS;
            v.coords.y = map_y_offset + y * mapS;
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
    pos.coords.x = map_x_offset + p.x - player_size/2;
    pos.coords.y = map_y_offset + p.y - player_size/2;
    pos.color = get_color_from_rgb(MAX_RED, MAX_GREEN, 0); /* yellow (1,1,0) */
    draw_rectangle(TEX_MAIN, 1, pos, player_size, player_size);

    /* Rotation: Line to represent where the player is looking */
    pos.coords.x = map_x_offset + p.x;
    pos.coords.y = map_y_offset + p.y;
    rot_end.coords.x = pos.coords.x + pdx*5;
    rot_end.coords.y = pos.coords.y + pdy*5;
    rot_end.color = pos.color;
    draw_line(TEX_MAIN, pos, rot_end);
    return;
}

static void draw_floor_and_ceiling(void)
{
    Vertex v;
    v.coords.x = TEX_MAIN->width*0.52f;
    v.coords.y = TEX_MAIN->height*0.05f;
    v.color = get_color_from_rgb(3, 1, 2);
    draw_rectangle(TEX_MAIN, 1, v, TEX_MAIN->width*0.47f, TEX_MAIN->height*0.45f);

    v.coords.y = TEX_MAIN->height*0.5f;
    v.color = get_color_from_rgb(3, 1, 3);
    draw_rectangle(TEX_MAIN, 1, v, TEX_MAIN->width*0.47f, TEX_MAIN->height*0.445f);
    return;
}

static void raycasting(void)
{
    int r, mx, my, mp, dof;
    float rx, ry, ra, xo, yo;
    /*
        "r" is "ray"
        "mx" is "x max"
        "my" is "y max"
        "mp" is "map position"
        "dof" is "depth of field"
        "rx" is "ray X": the X value where the ray first hits the closest horizontal line
        "ry" is "ray Y": the Y value where the ray first hits the closest horizontal line
        "ra" is "ray angle"
        "xo" is "X offset": the next X offset of the ray after the ray's first hit position
        "yo" is "Y offset": the next Y offset of the ray after the ray's first hit position
        "aTan" is "arc tangent"
        "nTan" is "negative tangent"
        "lineH" is "line height" for pseudo 3D
        "lineO" is "line offset" for pseudo 3D (to center the wall on the Y axis)
        "lineW" is "line width" for pseudo 3D
    */
    float tan, aTan, nTan;
    float disH, disV, disT;
    VectorF h, v;
    Vertex v1, v2;
    const int pov = 60;
    float ca, lineH, lineO;
    int lineW;

    /* `ra = pa` for center */
    ra = pa + pov/2 * RAD_1;
    ra = clamp_radians(ra);

    lineW = TEX_MAIN->width*0.5f / pov;

    for (r = pov; r > 0; --r)
    {
        tan = f_tan(ra);

        /* Check horizontal lines ------------------------------------------- */
        dof = 0;
        rx = 0;
        ry = 0;
        xo = 0;
        yo = 0;
        aTan = -1/tan;

        /* If ray is looking down */
        if (ra > RAD_180)
        {
            ry = (int)(p.y/mapS) * mapS;
            rx = (p.y - ry) * aTan + p.x;
            yo = -mapS;
            xo = -yo*aTan;
        }

        /* If ray is looking up */
        else if (ra < RAD_180)
        {
            ry = (int)((p.y+mapS)/mapS) * mapS;
            rx = (p.y - ry) * aTan + p.x;
            yo = mapS;
            xo = -yo*aTan;
        }

        /* From ray to map array index */
        h.x = p.x;
        h.y = p.y;
        disH = 1000000;
        while (dof < 8)
        {
            mx = (int)(rx/mapS);
            my = mapY - (int)(ry/mapS);
            if (ra < RAD_180) --my;
            mp = my*mapX+mx;
            if (mp >= 0 && mp < mapX*mapY && map[mp] == 1)
            {
                h.x = rx;
                h.y = ry;
                disH = get_distance(p,h);
                dof = 8;
            }
            else
            {
                rx += xo;
                ry += yo;
                ++dof;
            }
        }

        /* Check vertical lines ------------------------------------------- */
        dof = 0;
        rx = 0;
        ry = 0;
        xo = 0;
        yo = 0;
        nTan = -tan;

        /* If ray is looking left */
        if (ra > RAD_90 && ra < RAD_270)
        {
            rx = (int)(p.x/mapS) * mapS;
            ry = (p.x - rx) * nTan + p.y;
            xo = -mapS;
            yo = -xo*nTan;
        }

        /* If ray is looking right */
        else if (ra < RAD_90 || ra > RAD_270)
        {
            rx = (int)((p.x+mapS)/mapS) * mapS;
            ry = (p.x - rx) * nTan + p.y;
            xo = mapS;
            yo = -xo*nTan;
        }

        /* From ray to map array index */
        v.x = p.x;
        v.y = p.y;
        disV = 1000000;
        while (dof < 8)
        {
            mx = (int)(rx/mapS);
            my = mapY-1 - (int)(ry/mapS);
            if (ra > RAD_90 && ra < RAD_270) --mx;
            mp = my*mapX+mx;
            if (mp >= 0 && mp < mapX*mapY && map[mp] == 1)
            {
                v.x = rx;
                v.y = ry;
                disV = get_distance(p,v);
                dof = 8;
            }
            else
            {
                rx += xo;
                ry += yo;
                ++dof;
            }
        }

        /* Select the shortest ray ------------------------------------------ */
        if (disV <= disH)
        {
            rx = v.x;
            ry = v.y;
            disT = disV;
            v2.color = get_color_from_rgb(MAX_RED*0.4f, MAX_GREEN*0.2f, MAX_BLUE*0.6f);
        }
        else
        {
            rx = h.x;
            ry = h.y;
            disT = disH;
            v2.color = get_color_from_rgb(MAX_RED*0.5f, MAX_GREEN*0.2f, MAX_BLUE*0.6f);
        }

        /* Draw the ray ----------------------------------------------------- */
        v1.coords.x = map_x_offset + p.x;
        v1.coords.y = map_y_offset + p.y;
        v2.coords.x = map_x_offset + rx;
        v2.coords.y = map_y_offset + ry;
        v1.color = get_color_from_rgb(MAX_RED, 0, 0);
        draw_line(TEX_MAIN, v1, v2);

        /* Draw 3D ---------------------------------------------------------- */
        /* Fix fisheye effect */
        ca = pa - ra;
        ca = clamp_radians(ca);
        disT *= f_cos(ca);

        lineH = (TEX_MAIN->width*0.5f * mapS)/disT;
        if (lineH > TEX_MAIN->width*0.5f)
            lineH = TEX_MAIN->width*0.5f;
        lineO = TEX_MAIN->width*0.28f - lineH*0.5f;

        v1.color = v2.color;
        v1.coords.x = TEX_MAIN->width*0.52f + (pov-r)*lineW;
        v1.coords.y = lineO;
        draw_rectangle(TEX_MAIN, 1, v1, lineW, lineH);

        /* Go to next ray --------------------------------------------------- */
        ra -= RAD_1;
        ra = clamp_radians(ra);
    }
    return;
}

static float get_safe_angle(const float angle)
{
    /* The function is to prevent errors with f_tan in the raycasting */

    if (angle <= 0)
        return 0.0001f;

    if (float_equality(RAD_90, angle))
        return angle - 0.0001f;
    
    if (float_equality(RAD_180, angle))
        return angle - 0.0001f;
    
    if (float_equality(RAD_270, angle))
        return angle - 0.0001f;
    
    return angle;
}

