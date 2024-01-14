#include "../headers/interfaces.h"
#include "../headers/textures.h"

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
    draw_point(TEX_MAIN, 1, TEX_MAIN->width/2, TEX_MAIN->height/2);
    return;
}

