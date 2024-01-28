#include "../headers/interfaces.h"
#include "../headers/input.h"
#include "../headers/textures.h"
#include "../headers/draw_test.h"
#include "../headers/time.h"

Interface* active_interface = {0};
float global_x;
float global_y;
static Interface main_menu_interface;
static Interface game_interface;

static void reset_global_coordinates(void);
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
    reset_global_coordinates();
    return;
}

void update_global_coordinates(void)
{
    const float speed = 50.0f;
    global_x += movement_action[0] * speed * delta_time;
    global_y += movement_action[2] * speed * delta_time;
    return;
}

static void reset_global_coordinates(void)
{
    global_x = global_y = 0;
    return;
}

static void draw_main_menu(void)
{
    /*
    draw_test_gradient(TEX_MAIN);
    draw_test_shapes(TEX_MAIN);
    */
    draw_palette(TEX_MAIN);
    return;
}

static void draw_game(void)
{
    draw_test_rectangles(TEX_MAIN);
    draw_test_circles(TEX_MAIN);
    draw_test_lines(TEX_MAIN);
    draw_test_corners(TEX_MAIN);
    draw_test_center(TEX_MAIN);
    draw_test_gradient_line(TEX_MAIN);
    return;
}

