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

#define FOV 60
#define NBR_TEXTURES 3
/* Sprite size must be a power of two: */
#define SPR_W 64
#define SPR_H 64

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

typedef struct s_spr
{
    t_uivec2 size;
    GLubyte* buf;
} t_spr;

typedef struct s_map
{
    t_ivec2 size;
    t_vec2 start_pos;
    t_vec2 start_dir;
    double dof;
    double fog_width;
    t_color fog_color;
    int* data;
    int spr_len;
    t_spr** spr;
} t_map;

typedef struct s_player
{
    t_vec2 pos;
    t_vec2 dir;
    t_vec2 plane;
    int height;
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
    t_tex* tex[NBR_TEXTURES + 1];
    int curr_tex;
    t_player player;
    int movement_action[3];
    int rotation_action;
    t_map* map;
} t_manager;

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
void draw_sprite(t_tex* t, t_spr* s);

/* Fog ---------------------------------------------------------------------- */

void update_dof(t_map* m, double increment);
double get_fog_width(double dof);
void draw_floor_gradient(t_tex* t, double fog_width, t_color fog);
void draw_ceiling_gradient(t_tex* t, double fog_width, t_color fog);
void apply_wall_fog(t_color* wall, t_color fog, double dist, double dof);

/* Files -------------------------------------------------------------------- */

int is_digit(int c);
char* read_file(char* filepath);
t_spr* load_sprite(char* png_path);
t_spr* create_sprite(t_color c);
void free_sprite(t_spr* s);

/* Game --------------------------------------------------------------------- */

void draw_game(t_map* m);
void reset_global_coordinates(void);
void update_global_coordinates(void);

/* Input -------------------------------------------------------------------- */

void physical_key_callback(GLFWwindow* window, int key, int scancode, 
    int action, int mods);
void scroll_callback(GLFWwindow* window, double x_offset, double y_offset);

/* Maps --------------------------------------------------------------------- */

void initialize_maps(void);
void set_minimap_display(int remove_from_factor);
void draw_minimap(t_map* map);
void free_maps(void);

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
