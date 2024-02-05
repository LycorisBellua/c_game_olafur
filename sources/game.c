#include "../headers/game.h"
#include "../headers/textures.h"
#include "../headers/input.h"
#include "../headers/time.h"
#include "../headers/maths.h"
#include "../headers/coords.h"
#include "../headers/draw.h"
#include "../headers/maps.h"

static void draw_floor_and_ceiling(void);
static void raycasting(Map* m);
static float get_safe_angle(const float angle);

void draw_game(void)
{
    draw_floor_and_ceiling();
    raycasting(map_test);
    draw_minimap(map_test);
    return;
}

void reset_global_coordinates(void)
{
    player.pos.x = map_test->start_pos.x;
    player.pos.y = map_test->start_pos.y;
    player.angle = get_safe_angle(map_test->start_angle);
    player.delta.x = f_cos(player.angle)*5;
    player.delta.y = f_sin(player.angle)*5;
    return;
}

void update_global_coordinates(void)
{
    const float movement_speed = 20.0f;
    const float rotation_speed = 40.0f;

    player.angle += rotation_action * RAD_1 * rotation_speed * delta_time;
    if (rotation_action)
    {
        player.angle = clamp_radians(player.angle);
        player.angle = get_safe_angle(player.angle);

        /*
            The values returned by the sine and cosine operations are 
            very small, so multiply them by 5.
        */
        player.delta.x = f_cos(player.angle)*5;
        player.delta.y = f_sin(player.angle)*5;
    }

    /* Movement along the forward axis */
    player.pos.x += movement_action[2] * player.delta.x * movement_speed * 
        delta_time;
    player.pos.y += movement_action[2] * player.delta.y * movement_speed * 
        delta_time;

    /* Movement along the lateral axis */
    player.pos.x += movement_action[0] * player.delta.y * movement_speed * 
        delta_time;
    player.pos.y += movement_action[0] * -player.delta.x * movement_speed * 
        delta_time;
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

static void raycasting(Map* m)
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
    int lineW, screen_width;

    /* `ra = pa` for center */
    ra = player.angle + pov/2 * RAD_1;
    ra = clamp_radians(ra);

    lineW = 1;
    screen_width = TEX_MAIN->width*0.47f;

    for (r = screen_width; r > 0; --r)
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
            ry = (int)(player.pos.y/cell_len) * cell_len;
            rx = (player.pos.y - ry) * aTan + player.pos.x;
            yo = -cell_len;
            xo = -yo*aTan;
        }

        /* If ray is looking up */
        else if (ra < RAD_180)
        {
            ry = (int)((player.pos.y+cell_len)/cell_len) * cell_len;
            rx = (player.pos.y - ry) * aTan + player.pos.x;
            yo = cell_len;
            xo = -yo*aTan;
        }

        /* From ray to map array index */
        h.x = player.pos.x;
        h.y = player.pos.y;
        disH = 1000000;
        while (dof < 8)
        {
            mx = (int)(rx/cell_len);
            my = m->height - (int)(ry/cell_len);
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
            rx = (int)(player.pos.x/cell_len) * cell_len;
            ry = (player.pos.x - rx) * nTan + player.pos.y;
            xo = -cell_len;
            yo = -xo*nTan;
        }

        /* If ray is looking right */
        else if (ra < RAD_90 || ra > RAD_270)
        {
            rx = (int)((player.pos.x+cell_len)/cell_len) * cell_len;
            ry = (player.pos.x - rx) * nTan + player.pos.y;
            xo = cell_len;
            yo = -xo*nTan;
        }

        /* From ray to map array index */
        v.x = player.pos.x;
        v.y = player.pos.y;
        disV = 1000000;
        while (dof < 8)
        {
            mx = (int)(rx/cell_len);
            my = m->height-1 - (int)(ry/cell_len);
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

        lineH = (TEX_MAIN->width*0.5f * cell_len)/disT;
        if (lineH > TEX_MAIN->width*0.5f)
            lineH = TEX_MAIN->width*0.5f;
        lineO = TEX_MAIN->width*0.28f - lineH*0.5f;

        v1.color = v2.color;
        v1.coords.x = TEX_MAIN->width*0.52f + (screen_width-r)*lineW;
        v1.coords.y = lineO;
        draw_rectangle(TEX_MAIN, 1, v1, lineW, lineH);

        /* Go to next ray --------------------------------------------------- */
        ra -= RAD_1/(screen_width/pov);
        ra = clamp_radians(ra);
    }

    v1.coords.x = TEX_MAIN->width*0.52f;
    v1.coords.y = TEX_MAIN->height*0.05f;
    v1.color = 0;
    draw_rectangle(TEX_MAIN, 0, v1, TEX_MAIN->width*0.47f, TEX_MAIN->height*0.895f);
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

