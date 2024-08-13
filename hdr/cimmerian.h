#ifndef CIMMERIAN_H
#define CIMMERIAN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include "gl_functions.h"
#include "math.h"

#define MAP_CELL_LEN 64
#define MAX_CELL_AMOUNT 8
#define NBR_TEXTURES 3

typedef struct s_ivec2
{
    int x;
    int y;
} t_ivec2;

typedef struct s_vec2
{
    double x;
    double y;
} t_vec2;

typedef struct s_color
{
    GLubyte r;
    GLubyte g;
    GLubyte b;
    GLubyte a;
} t_color;

typedef struct s_vert
{
    t_ivec2 coord;
    t_color color;
} t_vert;

typedef struct s_ui t_ui;
typedef struct s_manager t_manager;
struct s_ui
{
    int nav_ui;
    void (*draw)(void);
    void (*reset_input)(void);
    t_ui* previous;
    t_ui* next;
};

typedef struct s_map
{
    t_ivec2 size;
    t_vec2 start_pos;
    double start_angle;
    int* data;
} t_map;

typedef struct s_player
{
    t_vec2 pos;
    t_vec2 delta;
    double angle;
    int map_index;
} t_player;

typedef struct s_tex
{
    GLuint id;
    t_ivec2 size;
    t_ivec2 real_size;
    int thickness;
    GLubyte* buf;
} t_tex;

typedef struct s_res
{
    t_ivec2 monitor_size;
    double aspect_ratio;

    t_ivec2 window_size_default;
    t_ivec2 window_size;
    t_ivec2 window_position;
    t_ivec2 fullscreen;
} t_res;

struct s_manager
{
    GLuint shader_program;
    double delta_time;
    double fps_count;
    t_res res;
    t_tex* tex[NBR_TEXTURES + 1];
    int curr_tex;
    t_ui* active_interface;
    t_ui main_menu_interface;
    t_ui game_interface;
    t_map* map;
    t_player player;
    int movement_action[3];
    int rotation_action;
};

extern t_manager man;

/* Colors ------------------------------------------------------------------- */

t_color get_color_rgba(GLubyte r, GLubyte g, GLubyte b, GLubyte a);
t_color get_color_hex(char* str, GLubyte alpha);
t_color get_alpha_blended_color(t_color prev, t_color new);

/* Coords ------------------------------------------------------------------- */

int get_coord_x(t_tex* t, double normalized);
int get_coord_y(t_tex* t, double normalized);
double get_coord_x_norm(t_tex* t, int coord);
double get_coord_y_norm(t_tex* t, int coord);
t_ivec2 get_direction(t_ivec2 v1, t_ivec2 v2);
t_vec2 get_direction_double(t_vec2 v1, t_vec2 v2);
double get_distance(t_vec2 a, t_vec2 b);

/* Draw --------------------------------------------------------------------- */

void draw_point(t_tex* t, t_color color, int x, int y);
void draw_line(t_tex* t, t_vert v1, t_vert v2);
void draw_rectangle(t_tex* t, t_vert v, t_ivec2 size);
void draw_rectangle_full(t_tex* t, t_vert v, t_ivec2 size);
void draw_circle(t_tex* t, t_vert center, int radius);
void draw_circle_full(t_tex* t, t_vert center, int radius);
void draw_circle_full_grad(t_tex* t, t_vert center, int radius, t_color edge);
void draw_shape(t_tex* t, t_vert arr[], int len);
void draw_shape_full(t_tex* t, t_vert arr[], int len);

void draw_test(void);
void draw_test_corners(t_tex* t);
void draw_test_center(t_tex* t);
void draw_test_lines(t_tex* t);
void draw_test_rectangles(t_tex* t);
void draw_test_circles(t_tex* t);
void draw_test_shapes(t_tex* t);
void draw_test_gradient(t_tex* t);

/* Files -------------------------------------------------------------------- */

int is_digit(int c);
char* read_file(char* filepath);

/* Game --------------------------------------------------------------------- */

/* Called from the "game" interface object */
void draw_game(void);
void reset_global_coordinates(void);

/* Called from the main loop */
void update_global_coordinates(void);

/* Input -------------------------------------------------------------------- */

void physical_key_callback(GLFWwindow* window, int key, int scancode, 
    int action, int mods);
void scroll_callback(GLFWwindow* window, double x_offset, double y_offset);

/* Interfaces --------------------------------------------------------------- */

void initialize_interfaces(void);
void set_active_interface(t_ui* interface);

/* Maps --------------------------------------------------------------------- */

void initialize_maps(void);
void set_minimap_display(int remove_from_factor);
void draw_minimap(t_map* map);
void free_maps(void);

/* Menu --------------------------------------------------------------------- */

/* Called from the "main menu" interface object */
void draw_main_menu(void);
void reset_menu_button_selection(void);

/* Events called from input.c */
void nav_ui_horizontal(int dir);
void nav_ui_vertical(int dir);
void nav_ui_confirm(GLFWwindow* window);
void nav_ui_cancel(GLFWwindow* window);

/* Mesh --------------------------------------------------------------------- */

void create_mesh(void);
void render_mesh(void);
void free_mesh(void);

/* Player ------------------------------------------------------------------- */

void reset_player_transform(t_map* m);
void update_player_transform(t_map* m);

/* Shader Program ----------------------------------------------------------- */

int create_shader_program(void);
void free_shader_program(void);

/* Textures ----------------------------------------------------------------- */

void create_textures(void);
void use_texture(t_tex* t);
void clear_drawing(t_tex* t);
void save_drawing(t_tex* t);
void free_textures(void);

/* Uniform ------------------------------------------------------------------ */

void create_uniform(void);
void free_uniform(void);

/* Windowing ---------------------------------------------------------------- */

GLFWwindow* get_window(char* title);
void toggle_fullscreen(GLFWwindow* window);

#endif
