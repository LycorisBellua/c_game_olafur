#include "../headers/game.h"
#include "../headers/maps.h"
#include "../headers/player.h"
#include "../headers/maths.h"
#include "../headers/textures.h"
#include "../headers/draw.h"

#define FOV 60
static float dist_proj_plane = 554;

static void draw_floor_and_ceiling(void);
static void raycasting(const Map* m);

void draw_game(void)
{
    draw_floor_and_ceiling();
    raycasting(map_test);
    draw_minimap(map_test);
    return;
}

void reset_global_coordinates(void)
{
    set_minimap_display(0);
    reset_player_transform(map_test);
    dist_proj_plane = TEX_MAIN->width / (2.0f * f_tan(DEG2RAD(FOV)/2));
    return;
}

static void draw_floor_and_ceiling(void)
{
    Vertex v;

    v.coords.x = 0;
    v.coords.y = 0;
    v.color = get_color_from_rgb(3, 1, 2);
    draw_rectangle(TEX_MAIN, 1, v, TEX_MAIN->width, TEX_MAIN->height*0.5f);

    v.coords.y = TEX_MAIN->height*0.5f;
    v.color = get_color_from_rgb(3, 1, 3);
    draw_rectangle(TEX_MAIN, 1, v, TEX_MAIN->width, TEX_MAIN->height*0.5f);
    return;
}

static void raycasting(const Map* m)
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
        "lineW" is "line width" for pseudo 3D
    */
    float tan, aTan, nTan;
    float disH, disV, disT;
    VectorF h, v;
    Vertex v1, v2;
    float ca, lineH;

    /* `ra = pa` for center */
    ra = player.angle + FOV/2.0f * RAD_1;
    ra = clamp_radians(ra);

    for (r = TEX_MAIN->width; r > 0; --r)
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
            ry = (int)(player.pos.y/MAP_CELL_LEN) * MAP_CELL_LEN;
            rx = (player.pos.y - ry) * aTan + player.pos.x;
            yo = -MAP_CELL_LEN;
            xo = -yo*aTan;
        }

        /* If ray is looking up */
        else if (ra < RAD_180)
        {
            ry = (int)((player.pos.y+MAP_CELL_LEN)/MAP_CELL_LEN) * MAP_CELL_LEN;
            rx = (player.pos.y - ry) * aTan + player.pos.x;
            yo = MAP_CELL_LEN;
            xo = -yo*aTan;
        }

        /* From ray to map array index */
        h.x = player.pos.x;
        h.y = player.pos.y;
        disH = 1000000;
        while (dof < 8)
        {
            mx = (int)(rx/MAP_CELL_LEN);
            my = m->height - (int)(ry/MAP_CELL_LEN);
            if (ra < RAD_180) --my;
            mp = my*m->width+mx;
            if (mp >= 0 && mp < m->width*m->height && m->data[mp] == 1)
            {
                h.x = rx;
                h.y = ry;
                disH = get_distance(player.pos,h);
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
            rx = (int)(player.pos.x/MAP_CELL_LEN) * MAP_CELL_LEN;
            ry = (player.pos.x - rx) * nTan + player.pos.y;
            xo = -MAP_CELL_LEN;
            yo = -xo*nTan;
        }

        /* If ray is looking right */
        else if (ra < RAD_90 || ra > RAD_270)
        {
            rx = (int)((player.pos.x+MAP_CELL_LEN)/MAP_CELL_LEN) * MAP_CELL_LEN;
            ry = (player.pos.x - rx) * nTan + player.pos.y;
            xo = MAP_CELL_LEN;
            yo = -xo*nTan;
        }

        /* From ray to map array index */
        v.x = player.pos.x;
        v.y = player.pos.y;
        disV = 1000000;
        while (dof < 8)
        {
            mx = (int)(rx/MAP_CELL_LEN);
            my = m->height-1 - (int)(ry/MAP_CELL_LEN);
            if (ra > RAD_90 && ra < RAD_270) --mx;
            mp = my*m->width+mx;
            if (mp >= 0 && mp < m->width*m->height && m->data[mp] == 1)
            {
                v.x = rx;
                v.y = ry;
                disV = get_distance(player.pos,v);
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

        /* Draw 3D ---------------------------------------------------------- */
        /* Fix fisheye effect */
        ca = player.angle - ra;
        ca = clamp_radians(ca);
        disT *= f_cos(ca);

        lineH = (MAP_CELL_LEN / disT) * dist_proj_plane;
        v1.coords.x = TEX_MAIN->width - r;
        v1.coords.y = (TEX_MAIN->height - lineH) / 2;
        v1.color = v2.color;
        draw_line_vertical(TEX_MAIN, v1, (TEX_MAIN->height + lineH) / 2);

        /* Go to next ray --------------------------------------------------- */
        ra -= RAD_1/(TEX_MAIN->width/FOV);
        ra = clamp_radians(ra);
    }
    return;
}

