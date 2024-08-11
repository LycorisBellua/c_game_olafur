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

typedef struct
{
    int x;
    int y;
} Vector;

typedef struct
{
    double x;
    double y;
} VectorF;

typedef struct
{
    GLubyte r;
    GLubyte g;
    GLubyte b;
    GLubyte a;
} Color;

typedef struct
{
    Vector coords;
    Color color;
} Vertex;

typedef struct Interface Interface;
typedef struct Manager Manager;
struct Interface
{
    int nav_ui;
    void (*draw)(void);
    void (*reset_input)(void);
    Interface* previous;
    Interface* next;
};

typedef struct
{
    int width;
    int height;
    VectorF start_pos;
    double start_angle;
    int* data;
} Map;

typedef struct
{
    VectorF pos;
    VectorF delta;
    double angle;
    int map_index;
} Player;

typedef struct
{
    GLuint id;
    int real_width;
    int real_height;
    int thickness;
    int width;
    int height;
    GLubyte* buffer;
} Texture;

typedef struct
{
    int monitor_width;
    int monitor_height;
    double aspect_ratio;

    int window_width_default;
    int window_height_default;
    int window_width;
    int window_height;
    int window_position_x;
    int window_position_y;
    int fullscreen_x;
    int fullscreen_y;
} Resolution;

struct Manager
{
    GLuint shader_program;
    double delta_time;
    double fps_count;
    Resolution res;
    Texture* tex[NBR_TEXTURES + 1];
    int curr_tex;
    Interface* active_interface;
    Interface main_menu_interface;
    Interface game_interface;
    Map* map;
    Player player;
    int movement_action[3];
    int rotation_action;
};

extern Manager man;

/* Colors ------------------------------------------------------------------- */

Color get_color_rgba(const GLubyte r, const GLubyte g, const GLubyte b,
    const GLubyte a);
Color get_color_hex(const char* str, const GLubyte alpha);
Color get_alpha_blended_color(Color prev, Color new);
int is_digit(const int c);

/* Coords ------------------------------------------------------------------- */

int get_coord_x(Texture* t, double normalized);
int get_coord_y(Texture* t, double normalized);
double get_coord_x_norm(Texture* t, int coord);
double get_coord_y_norm(Texture* t, int coord);
int is_coord_out_of_bounds(int axis_length, int coord);
Vector get_direction(Vector v1, Vector v2);
VectorF get_direction_double(VectorF v1, VectorF v2);
double get_distance(const VectorF a, const VectorF b);

/* Draw --------------------------------------------------------------------- */

void draw_point(Texture* t, Color color, int x, int y);
void draw_line(Texture* t, Vertex v1, Vertex v2);
void draw_rectangle(Texture* t, Vertex v, Vector size);
void draw_rectangle_full(Texture* t, Vertex v, Vector size);
void draw_circle(Texture* t, Vertex center, int radius);
void draw_circle_full(Texture* t, Vertex center, int radius);
void draw_circle_full_grad(Texture* t, Vertex center, int radius, Color edge);
void draw_shape(Texture* t, Vertex arr[], int len);
void draw_shape_full(Texture* t, Vertex arr[], int len);

void draw_test(void);
void draw_test_corners(Texture* t);
void draw_test_center(Texture* t);
void draw_test_lines(Texture* t);
void draw_test_rectangles(Texture* t);
void draw_test_circles(Texture* t);
void draw_test_shapes(Texture* t);
void draw_test_gradient(Texture* t);

/* Files -------------------------------------------------------------------- */

char* read_file(const char* filepath);

/* Game --------------------------------------------------------------------- */

/* Called from the "game" interface object */
void draw_game(void);
void reset_global_coordinates(void);

/* Called from the main loop */
void update_global_coordinates(void);

/* Input -------------------------------------------------------------------- */

void physical_key_callback(GLFWwindow* window, const int key, 
    const int scancode, const int action, const int mods);
void scroll_callback(GLFWwindow* window, double x_offset, double y_offset);

/* Interfaces --------------------------------------------------------------- */

void initialize_interfaces(void);
void set_active_interface(Interface* interface);

/* Maps --------------------------------------------------------------------- */

void initialize_maps(void);
void set_minimap_display(const int remove_from_factor);
void draw_minimap(const Map* map);
void free_maps(void);

/* Menu --------------------------------------------------------------------- */

/* Called from the "main menu" interface object */
void draw_main_menu(void);
void reset_menu_button_selection(void);

/* Events called from input.c */
void nav_ui_horizontal(const int dir);
void nav_ui_vertical(const int dir);
void nav_ui_confirm(GLFWwindow* window);
void nav_ui_cancel(GLFWwindow* window);

/* Mesh --------------------------------------------------------------------- */

void create_mesh(void);
void render_mesh(void);
void free_mesh(void);

/* Player ------------------------------------------------------------------- */

void reset_player_transform(const Map* m);
void update_player_transform(const Map* m);

/* Shader Program ----------------------------------------------------------- */

int create_shader_program(void);
void free_shader_program(void);

/* Textures ----------------------------------------------------------------- */

void create_textures(void);
void use_texture(const Texture* t);
void clear_drawing(Texture* t);
void save_drawing(const Texture* t);
void free_textures(void);

/* Uniform ------------------------------------------------------------------ */

void create_uniform(void);
void free_uniform(void);

/* Windowing ---------------------------------------------------------------- */

GLFWwindow* get_window(const char* title);
void toggle_fullscreen(GLFWwindow* window);

#endif
