#include "../headers/interfaces.h"
#include "../headers/meshes.h"
#include "../headers/rendering.h"

Interface* active_interface = {0};

Interface main_menu_interface;
Interface game_interface;

void initialize_interfaces(void)
{
    active_interface = &main_menu_interface;

    main_menu_interface.render = render_main_menu;
    main_menu_interface.previous = 0;
    main_menu_interface.next = &game_interface;

    game_interface.render = render_game;
    game_interface.previous = &main_menu_interface;
    game_interface.next = 0;
    return;
}

void set_active_interface(Interface* interface)
{
    active_interface = interface;

    if (interface == &main_menu_interface)
        straighten_pitch();
    else
        set_pitch_back();
    return;
}

