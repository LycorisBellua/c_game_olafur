#include "cimmerian.h"

Interface* active_interface = {0};
static Interface main_menu_interface;
static Interface game_interface;

void initialize_interfaces(void)
{
    main_menu_interface.nav_ui = 1;
    main_menu_interface.draw = draw_main_menu;
    main_menu_interface.reset_input = reset_menu_button_selection;
    main_menu_interface.previous = 0;
    main_menu_interface.next = &game_interface;

    game_interface.nav_ui = 0;
    /* game_interface.draw = draw_game; */
    game_interface.draw = draw_test;
    game_interface.reset_input = reset_global_coordinates;
    game_interface.previous = &main_menu_interface;
    game_interface.next = 0;

    set_active_interface(&main_menu_interface);
    return;
}

void set_active_interface(Interface* interface)
{
    if (interface)
        interface->reset_input();
    active_interface = interface;
    return;
}
