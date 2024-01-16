#include "../headers/interfaces.h"
#include "../headers/textures.h"
#include "../headers/colors.h"
#include "../headers/draw_rectangle.h"

Interface* active_interface = {0};
static Interface main_menu_interface;
static Interface game_interface;

static void draw_main_menu(void);
static void draw_game(void);

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

    /*
    draw_line(TEX_MAIN, TEX_MAIN->width/100*10, TEX_MAIN->height/100*10, 
        TEX_MAIN->width/100*50, TEX_MAIN->height/100*50);
    */

    color_default = &colors[COLOR_WHITE];
    draw_rectangle(TEX_MAIN, 1, TEX_MAIN->width*0.1f-1, TEX_MAIN->height*0.33f-1, 
        100, 100);
    color_default = &colors[COLOR_RED];
    draw_rectangle(TEX_MAIN, 0, TEX_MAIN->width*0.1f, TEX_MAIN->height*0.33f-2, 
        100, 100);
    return;
}

