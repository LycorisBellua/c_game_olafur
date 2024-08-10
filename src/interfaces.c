#include "cimmerian.h"

void initialize_interfaces(void)
{
    man.main_menu_interface.nav_ui = 1;
    man.main_menu_interface.draw = draw_main_menu;
    man.main_menu_interface.reset_input = reset_menu_button_selection;
    man.main_menu_interface.previous = 0;
    man.main_menu_interface.next = &man.game_interface;

    man.game_interface.nav_ui = 0;
    man.game_interface.draw = draw_test; /* draw_game; */
    man.game_interface.reset_input = reset_global_coordinates;
    man.game_interface.previous = &man.main_menu_interface;
    man.game_interface.next = 0;

    set_active_interface(&man.main_menu_interface);
    return;
}

void set_active_interface(Interface* interface)
{
    if (interface)
        interface->reset_input();
    man.active_interface = interface;
    return;
}
