#ifndef CIMMERIAN_H
# define CIMMERIAN_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <strings.h>
# include <ctype.h>
# include <fcntl.h>
# include <unistd.h>
# include <sys/time.h>
# include <GL/gl.h>
# include "glfw3.h"
# include "gl_functions.h"
# include "math.h"
# include "lodepng.h"

# define FOV 60
# define NBR_FRAMES 3
# define SPRITE_LEN 20
# define NBR_OBJ 20
# define DEFAULT_MOVE_SPEED 2.0
# define DEFAULT_ROTATE_SPEED 0.25

typedef unsigned char	t_ubyte;

typedef struct s_list
{
	struct s_list	*prev;
	void			*data;
	struct s_list	*next;
}	t_list;

typedef struct s_vec2
{
	double	x;
	double	y;
}	t_vec2;

typedef struct s_ivec2
{
	int	x;
	int	y;
}	t_ivec2;

typedef struct s_uivec2
{
	unsigned int	x;
	unsigned int	y;
}	t_uivec2;

typedef struct s_color
{
	t_ubyte	r;
	t_ubyte	g;
	t_ubyte	b;
	t_ubyte	a;
}	t_color;

typedef struct s_vert
{
	t_ivec2	coord;
	t_color	color;
}	t_vert;

typedef struct s_spr
{
	char	*id;
	t_ivec2	size;
	t_ivec2	shadow_offset;
	int		still_frame;
	size_t	cycle_len;
	size_t	cycle_index;
	long	elapsed_time_in_ms;
	long	cycle_time_in_ms;
	t_color	**cycle;
	t_color	**cycle_shadow;
	t_color	*average_color;
	int		*is_see_through;
}	t_spr;

typedef struct s_img_seg
{
	char	*id;
	t_ivec2	size;
	t_ivec2	shadow_offset;
	int		still_frame;
	long	cycle_time_in_ms;
	size_t	cycle_len;
	t_ivec2	*cycle;
}	t_img_seg;

typedef struct s_img
{
	char		*path;
	char		*path_shadow;
	t_ivec2		size;
	t_color		*buf;
	t_color		*buf_shadow;
	size_t		segment_len;
	t_img_seg	*seg;
}	t_img;

typedef struct s_ray
{
	t_vec2	ray_dir;
	t_ivec2	m_index;
	int		side;
	double	perp_wall_dist;
	int		line_height;
	int		line_height_cubic;
	t_ivec2	unclamped_line_height;
	t_ivec2	coord1;
	t_ivec2	coord2;
}	t_ray;

typedef struct s_cell
{
	int		is_obstacle;
	int		is_door;
	int		is_goal;
	int		is_indoors;
	double	height;
	t_spr	*tex_floor;
	t_spr	*tex_ceiling;
	t_spr	*tex_north;
	t_spr	*tex_east;
	t_spr	*tex_south;
	t_spr	*tex_west;
}	t_cell;

typedef struct s_obj
{
	t_vec2	pos;
	t_spr	*spr;
}	t_obj;

typedef struct s_map
{
	t_ivec2	size;
	t_vec2	start_pos;
	t_vec2	start_dir;
	double	dof;
	double	fog_width;
	t_color	fog_color;
	t_spr	*skybox;
	t_img	*background;
	t_cell	*cells;
	t_obj	*objects;
	t_ivec2	minimap_offset;
	t_ivec2	minimap_center;
	int		minimap_radius;
	int		minimap_zoom;
	int		minimap_cell_amount;
}	t_map;

typedef struct s_player
{
	t_vec2	pos;
	t_vec2	dir;
	t_vec2	plane;
}	t_player;

typedef struct s_frame
{
	GLuint	id;
	t_ivec2	size;
	t_ivec2	real_size;
	int		thickness;
	t_color	*buf;
}	t_frame;

typedef struct s_res
{
	t_ivec2	monitor_size;
	double	aspect_ratio;
	double	h_mod;
	t_ivec2	window_size_default;
	t_ivec2	window_size;
	t_ivec2	window_position;
	t_ivec2	viewport_size;
	t_ivec2	viewport_offset;
	t_ivec2	fullscreen;
}	t_res;

typedef struct s_manager
{
	GLuint		shader_program;
	GLint		uniform_loc;
	double		dt;
	long		dt_ms;
	int			fps;
	t_res		res;
	t_frame		*frame[NBR_FRAMES + 1];
	int			curr_frame;
	double		tex_in_dof;
	t_player	player;
	double		move_speed;
	double		rotate_speed;
	t_ivec2		move_action;
	int			rotate_action;
	int			l_click_action;
	int			r_click_action;
	t_ivec2		cursor;
	t_spr		sprites[SPRITE_LEN];
	t_map		*map;
}	t_manager;

extern t_manager	g_man;

/* Linked List -------------------------------------------------------------- */

t_list		*list_last(t_list *list);
t_list		*list_new(void *data);
void		list_add_front(t_list **list, t_list *new);
void		list_add_back(t_list **list, t_list *new);
void		list_del_one(t_list **list, void (*del)(void*));

/* Colors ------------------------------------------------------------------- */

t_color		get_color_rgba(t_ubyte r, t_ubyte g, t_ubyte b, t_ubyte a);
t_color		get_color_hex(const char *str, t_ubyte alpha);
t_color		get_alpha_blended_color(t_color prev, t_color new);
t_color		get_frame_color(t_frame *f, int x, int y);
t_color		calculate_average_color(t_color *buf, size_t len);

/* Vectors ------------------------------------------------------------------ */

void		set_vec2(t_vec2 *v, double x, double y);
void		set_ivec2(t_ivec2 *v, int x, int y);

/* Draw --------------------------------------------------------------------- */

void		draw_pixel(t_color *buf, t_color c, t_ivec2 coord, t_ivec2 size);
void		draw_point(t_frame *f, t_color color, int x, int y);
void		draw_line(t_frame *f, t_vert v1, t_vert v2);
void		draw_rectangle(t_frame *f, t_vert v, t_ivec2 size);
void		draw_rectangle_full(t_frame *f, t_vert v, t_ivec2 size);
void		draw_circle(t_frame *f, t_vert center, int radius);
void		draw_circle_full(t_frame *f, t_vert center, int radius);
void		draw_circle_full_gradient(t_frame *f, t_vert center, int radius,
				t_color edge);
void		draw_shape(t_frame *f, t_vert arr[], int len);
void		draw_shape_full(t_frame *f, t_vert arr[], int len);
void		draw_image(t_frame *f, t_img *img);
void		draw_image_with_x_offset(t_frame *f, t_img *img, int x_offset);
void		draw_font_default(t_frame *frame, t_ivec2 *pos, char *str);
void		draw_sprite(t_frame *frame, t_spr *sprite, t_ivec2 pos);
void		draw_cursor(t_frame *frame, t_spr *sprite, t_ivec2 p, int cyc);
t_spr		*get_sprite(char *id);
void		advance_all_sprite_cycles(long dt_ms);

/* Fog ---------------------------------------------------------------------- */

void		update_dof(t_map *m, double increment);
double		get_fog_width(double dof);
void		apply_wall_shadow(t_color *wall, t_color c, int y, t_ivec2 height);
void		apply_corner_shadow(t_color *wall, t_color c, int img_coord_x,
				int img_size_x);
void		apply_wall_fog(t_color *wall, t_color c, double dist, double dof);
void		update_background(t_map *m, t_img *bg);

/* Files -------------------------------------------------------------------- */

char		*read_file(const char *filepath);
char		*gnl(int fd);
char		*strjoin(char const *s1, char const *s2);
char		*itoa_dec(int number);

/* JSON --------------------------------------------------------------------- */

char		**get_json_content(const char *filepath);
void		free_json_content(char **content);
int			set_img_file_obj(t_img *file, char **lines);
void		free_and_reset_img_file_obj(t_img *file);
int			is_field(char *line, const char *field);
char		*get_string_value(char *line);
int			get_int_value(char *line);
t_ivec2		get_ivec2_value(char *line);
void		parse_segments(t_img *file, char **lines, size_t *i);

/* Images ------------------------------------------------------------------- */

t_img		*load_image_from_file(const char *png_path);
t_img		*create_image(t_color c, t_ivec2 size);
t_spr		*compose_skybox(t_spr *src, t_color fog);
t_img		*compose_background(t_map *m);
void		draw_background(t_frame *f, t_map *m);
void		apply_vertical_gradient(t_img *img, t_color color);
void		free_image(t_img *s);
void		add_outline_to_font(t_spr *font);
int			create_sprites_from_file(t_img *file, size_t *i_spr);
void		free_sprites(void);
void		free_sprite(t_spr *s);
int			set_sprite_array(char *path);
int			calculate_sprite_average_color(t_spr *s);
t_spr		*duplicate_sprite(const char *dst_id, t_spr *src);

/* Game --------------------------------------------------------------------- */

void		set_dt_and_fps(void);
void		display_fps(t_frame *f, t_ivec2 pos);
void		run_game_loop(t_map *m);
void		door_routine(t_map *m);
void		reset_global_coordinates(void);
void		update_global_coordinates(void);

/* Raycasting --------------------------------------------------------------- */

void		raycasting(t_frame *f, t_map *m);
void		cast_floor(t_frame *f, t_map *m);
void		cast_ceiling_x(t_frame *f, t_map *m, double *z_buffer, int x);
void		draw_wall(t_frame *f, t_map *m, t_ray *r);
int			is_corner(t_map *m, t_ray *r, int img_coord_x, int img_size_x);
void		sort_objects(int *object_order, double *object_dist,
				int object_amount);
void		cast_objects(t_frame *f, t_map *m, double *z_buffer,
				int *object_order, double *object_dist, int x);

/* Input -------------------------------------------------------------------- */

void		physical_key_callback(GLFWwindow *window, int key, int scancode,
				int action, int mods);
void		scroll_callback(GLFWwindow *window, double x_offset,
				double y_offset);
void		mouse_callback(GLFWwindow *window, int button, int action,
				int mods);
void		cursor_pos_callback(GLFWwindow *window, double xpos, double ypos);

/* Maps --------------------------------------------------------------------- */

int			initialize_maps(void);
void		free_maps(void);
void		draw_minimap(t_frame *f, t_map *m);
void		decrease_minimap_zoom(void);
void		increase_minimap_zoom(void);

/* Mesh --------------------------------------------------------------------- */

int			create_mesh(void);
void		render_mesh(void);
void		free_mesh(void);

/* Transform ---------------------------------------------------------------- */

void		reset_player_transform(t_map *m);
void		update_player_transform(t_map *m);
void		rotate_player(double angle);
void		echolocation(int has_player_moved);

/* Shader Program ----------------------------------------------------------- */

int			create_shader_program(void);
void		free_shader_program(void);

/* Frames ------------------------------------------------------------------- */

int			create_frames(void);
void		use_frame(t_frame *f);
void		clear_drawing(t_frame *f);
void		save_drawing(t_frame *f);
void		free_frames(void);

/* Uniform ------------------------------------------------------------------ */

int			create_uniform(void);
void		free_uniform(void);

/* Windowing ---------------------------------------------------------------- */

GLFWwindow	*get_window(const char *title);
void		toggle_fullscreen(GLFWwindow *window);

#endif
