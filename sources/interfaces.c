#include "../headers/interfaces.h"
#include "../headers/shader_handling.h"
#include "../headers/meshes.h"
#include "../headers/uniforms.h"
#include "../headers/camera.h"

Interface* active_interface = {0};
static Interface main_menu_interface;
static Interface game_interface;

static void render_viewport(void);

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

    if (interface)
        camera_moves = interface != &main_menu_interface;
    return;
}

void render_main_menu(void)
{
    render_viewport();

    use_shader_program(shader_program_ui);
    UNIFORM_SINGLE_COLOR->activate(UNIFORM_SINGLE_COLOR, 0);
    render_mesh(meshes[SHAPE_TRIANGLE], GL_TRIANGLES);
    use_shader_program(shader_program_world);
    return;
}

void render_game(void)
{
    render_viewport();

    UNIFORM_SINGLE_COLOR->activate(UNIFORM_SINGLE_COLOR, 0);
    render_mesh(meshes[SHAPE_FLOOR], GL_TRIANGLES);
    render_mesh(meshes[SHAPE_CUBE], GL_TRIANGLES);

    UNIFORM_SINGLE_COLOR->activate(UNIFORM_SINGLE_COLOR, 1);
    render_mesh(meshes[SHAPE_CUBE], GL_LINE_LOOP);
    return;
}

static void render_viewport(void)
{
    use_shader_program(shader_program_ui);
    render_mesh(meshes[SHAPE_VIEWPORT], GL_TRIANGLES);
    use_shader_program(shader_program_world);
    return;
}

