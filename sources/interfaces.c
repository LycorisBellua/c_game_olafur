#include "../headers/interfaces.h"
#include "../headers/textures.h"
#include "../headers/colors.h"
#include "../headers/draw.h"

Interface* active_interface = {0};
static Interface main_menu_interface;
static Interface game_interface;

static void draw_main_menu(void);
static void draw_game(void);

/* Test functions */
static void draw_corners(Texture* t);
static void draw_center(Texture* t);
static void draw_gradient(Texture* t);

void initialize_interfaces(void)
{
    set_active_interface(&main_menu_interface);

    main_menu_interface.draw = draw_main_menu;
    main_menu_interface.previous = 0;
    main_menu_interface.next = &game_interface;

    game_interface.draw = draw_game;
    game_interface.previous = &main_menu_interface;
    game_interface.next = 0;
    return;
}

void set_active_interface(Interface* interface)
{
    active_interface = interface;
    return;
}

static void draw_main_menu(void)
{
    draw_gradient(TEX_MAIN);
    return;
}

static void draw_game(void)
{
    draw_corners(TEX_MAIN);
    draw_center(TEX_MAIN);

    draw_line(TEX_MAIN, TEX_MAIN->width/100*10, TEX_MAIN->height/100*10, 
        TEX_MAIN->width/100*50, TEX_MAIN->height/100*10);

    color_default = colors[COLOR_WHITE];
    draw_rectangle(TEX_MAIN, 1, TEX_MAIN->width*0.1f-1, TEX_MAIN->height*0.33f-1, 
        100, 100);

    color_default = colors[COLOR_RED];
    draw_rectangle(TEX_MAIN, 0, TEX_MAIN->width*0.1f, TEX_MAIN->height*0.33f-2, 
        100, 100);
    return;
}

/* Test functions ---------------------------------------------------------- */

static void draw_corners(Texture* t)
{
    /* Bottom left */
    color_default = colors[COLOR_WHITE];
    draw_point(t, 0, 0);
    /* Top left */
    color_default = colors[COLOR_RED];
    draw_point(t, 0, t->height - 1);
    /* Top right */
    color_default = colors[COLOR_GREEN];
    draw_point(t, t->width - 1, t->height - 1);
    /* Bottom right */
    color_default = colors[COLOR_BLUE];
    draw_point(t, t->width - 1, 0);

    color_default = colors[COLOR_WHITE];
    return;
}

static void draw_center(Texture* t)
{
    /* Bottom left */
    color_default = colors[COLOR_WHITE];
    draw_point(t, t->width/2 -1, t->height/2 -1);
    /* Top left */
    color_default = colors[COLOR_RED];
    draw_point(t, t->width/2 -1, t->height/2);
    /* Top right */
    color_default = colors[COLOR_GREEN];
    draw_point(t, t->width/2, t->height/2);
    /* Bottom right */
    color_default = colors[COLOR_BLUE];
    draw_point(t, t->width/2, t->height/2 -1);

    color_default = colors[COLOR_WHITE];
    return;
}

static void draw_gradient(Texture* t)
{
    int x, y;
    GLubyte color[4];
    color[2] = 255/2;
    color[3] = 255;

    color_default = color;

    for (y = 0; y < t->height; ++y)
    {
        for (x = 0; x < t->width; ++x)
        {
            color[0] = x * 255 / t->width;
            color[1] = y * 255 / t->height;
            draw_point(t, x, y);
        }
    }

    color_default = colors[COLOR_WHITE];
    return;
}

