#ifndef CIMMERIAN_H
#define CIMMERIAN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <GL/gl.h>
#include "glfw3.h"
#include "gl_functions.h"
#include "math.h"
#include "lodepng.h"

#define FOV 60
#define NBR_FRAMES 3

typedef struct s_list
{
    struct s_list* prev;
    void* data;
    struct s_list* next;
} t_list;

typedef struct s_vec2
{
    double x;
    double y;
} t_vec2;

typedef struct s_ivec2
{
    int x;
    int y;
} t_ivec2;

typedef struct s_uivec2
{
    unsigned int x;
    unsigned int y;
} t_uivec2;

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

typedef struct s_img
{
    int is_see_through;
    t_ivec2 size;
    t_color average_color;
    GLubyte* buf;
} t_img;

typedef struct s_ray
{
    t_vec2 ray_dir;
    t_ivec2 m_index;
    int side;
    double perp_wall_dist;
    int line_height;
    t_ivec2 coord1;
    t_ivec2 coord2;
    t_list* alpha;
} t_ray;

typedef struct s_cell
{
    int is_obstacle;
    int is_door;
    t_img* tex_floor;
    t_img* tex_ceiling;
    t_img* tex_north;
    t_img* tex_east;
    t_img* tex_south;
    t_img* tex_west;
} t_cell;

typedef struct s_map
{
    t_ivec2 size;
    t_vec2 start_pos;
    t_vec2 start_dir;
    double dof;
    double fog_width;
    t_color fog_color;
    int img_len;
    t_img** img;
    t_cell* cells;
} t_map;

typedef struct s_player
{
    t_vec2 pos;
    t_vec2 dir;
    t_vec2 plane;
    int height;
} t_player;

typedef struct s_frame
{
    GLuint id;
    t_ivec2 size;
    t_ivec2 real_size;
    int thickness;
    GLubyte* buf;
} t_frame;

typedef struct s_res
{
    t_ivec2 monitor_size;
    double aspect_ratio;
    double h_mod;
    t_ivec2 window_size_default;
    t_ivec2 window_size;
    t_ivec2 window_position;
    t_ivec2 fullscreen;
} t_res;

typedef struct s_manager
{
    GLuint shader_program;
    double delta_time;
    double fps_count;
    t_res res;
    t_frame* frame[NBR_FRAMES + 1];
    int curr_frame;
    double tex_in_dof;
    t_player player;
    int movement_action[3];
    int rotation_action;
    t_map* map;
} t_manager;

extern t_manager man;

/* Linked List -------------------------------------------------------------- */

t_list* list_last(t_list *list);
t_list* list_new(void* data);
void list_add_front(t_list** list, t_list* new);
void list_add_back(t_list** list, t_list* new);
void list_del_one(t_list** list, void (*del)(void*));

/* Colors ------------------------------------------------------------------- */

t_color get_color_rgba(GLubyte r, GLubyte g, GLubyte b, GLubyte a);
t_color get_color_hex(char* str, GLubyte alpha);
t_color get_alpha_blended_color(t_color prev, t_color new);
t_color calculate_average_color(t_img *img);
t_color get_frame_color(t_frame* f, int x, int y);

/* Coords ------------------------------------------------------------------- */

void set_vec2(t_vec2* coord, double x, double y);
void set_ivec2(t_ivec2* coord, int x, int y);
int get_coord_x(t_frame* f, double normalized);
int get_coord_y(t_frame* f, double normalized);
double get_coord_x_norm(t_frame* f, int coord);
double get_coord_y_norm(t_frame* f, int coord);
t_ivec2 get_direction(t_ivec2 v1, t_ivec2 v2);
t_vec2 get_direction_double(t_vec2 v1, t_vec2 v2);

/* Draw --------------------------------------------------------------------- */

void draw_point(t_frame* f, t_color color, int x, int y);
void draw_line(t_frame* f, t_vert v1, t_vert v2);
void draw_rectangle(t_frame* f, t_vert v, t_ivec2 size);
void draw_rectangle_full(t_frame* f, t_vert v, t_ivec2 size);
void draw_circle(t_frame* f, t_vert center, int radius);
void draw_circle_full(t_frame* f, t_vert center, int radius);
void draw_circle_full_grad(t_frame* f, t_vert center, int radius, t_color edge);
void draw_shape(t_frame* f, t_vert arr[], int len);
void draw_shape_full(t_frame* f, t_vert arr[], int len);
void draw_image(t_frame* f, t_img* img);
void draw_image_with_x_offset(t_frame* f, t_img* img, int x_offset);

/* Fog ---------------------------------------------------------------------- */

void update_dof(t_map* m, double increment);
double get_fog_width(double dof);
void draw_floor(t_frame* f, double fog_width, t_color fog);
void draw_skybox(t_frame* f, t_img* skybox, double fog_width, t_color fog);
void apply_wall_fog(t_color* wall, t_color fog, double dist, double dof);

/* Files -------------------------------------------------------------------- */

char* read_file(char* filepath);

/* Images ------------------------------------------------------------------- */

t_img* load_image_from_file(char* png_path);
t_img* create_image(t_color c);
void apply_vertical_gradient(t_img* img, t_color color);
void free_image(t_img* s);

/* Game --------------------------------------------------------------------- */

void door_routine(t_map* m);
void draw_game(t_map* m);
void draw_wall(t_map* m, t_frame* f, t_ray* r);
void reset_global_coordinates(void);
void update_global_coordinates(void);

/* Input -------------------------------------------------------------------- */

void physical_key_callback(GLFWwindow* window, int key, int scancode, 
    int action, int mods);
void scroll_callback(GLFWwindow* window, double x_offset, double y_offset);

/* Maps --------------------------------------------------------------------- */

int initialize_maps(void);
void set_minimap_display(int remove_from_factor);
void draw_minimap(t_map* map);
void free_maps(void);

/* Mesh --------------------------------------------------------------------- */

int create_mesh(void);
void render_mesh(void);
void free_mesh(void);

/* Player ------------------------------------------------------------------- */

void reset_player_transform(t_map* m);
void update_player_transform(t_map* m);
double get_angle_from_dir(t_vec2 dir);

/* Shader Program ----------------------------------------------------------- */

int create_shader_program(void);
void free_shader_program(void);

/* Frames ------------------------------------------------------------------- */

int create_frames(void);
void use_frame(t_frame* f);
void clear_drawing(t_frame* f);
void save_drawing(t_frame* f);
void free_frames(void);

/* Uniform ------------------------------------------------------------------ */

int create_uniform(void);
void free_uniform(void);

/* Windowing ---------------------------------------------------------------- */

GLFWwindow* get_window(char* title);
void toggle_fullscreen(GLFWwindow* window);

#endif
