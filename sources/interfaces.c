#include "../headers/interfaces.h"
#include "../headers/meshes.h"

Interface* active_interface = {0};
static Interface main_menu_interface;
static Interface game_interface;

static void render_main_menu(void);
static void render_game(void);

void initialize_interfaces(void)
{
    set_active_interface(&main_menu_interface);

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
    return;
}

static void render_main_menu(void)
{
    render_mesh(MESH_VIEWPORT, GL_TRIANGLES);
    return;
}

static void render_game(void)
{
    render_mesh(MESH_VIEWPORT, GL_TRIANGLES);
    return;
}

