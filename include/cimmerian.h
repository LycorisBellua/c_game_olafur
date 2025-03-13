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

# define TITLE "Cimmerian"
# define RES_WIDTH  640
# define RES_HEIGHT 360
# define FOV 60
# define NBR_FRAMES 3
# define NBR_IMG 20
# define NBR_SPR 20
# define DEFAULT_MOVE_SPEED 2.0
# define DEFAULT_ROTATE_SPEED 0.25
# define DEFAULT_PLAYER_RADIUS 0.25
# define INF 1e30

typedef unsigned char	t_ubyte;

typedef struct s_list
{
	struct s_list	*prev;
	void			*data;
	struct s_list	*next;
}	t_list;

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

typedef struct s_img
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
}	t_img;

typedef struct s_png_seg
{
	char	*id;
	t_ivec2	size;
	t_ivec2	shadow_offset;
	int		still_frame;
	long	cycle_time_in_ms;
	size_t	cycle_len;
	t_ivec2	*cycle;
}	t_png_seg;

typedef struct s_png
{
	char		*path;
	char		*path_shadow;
	t_ivec2		size;
	t_color		*buf;
	t_color		*buf_shadow;
	size_t		segment_len;
	t_png_seg	*seg;
}	t_png;

typedef struct s_ray
{
	t_vec2	ray_dir;
	t_vec2	delta_dist;
	t_ivec2	m_index;
	t_ivec2	step;
	t_vec2	side_dist;
	int		side;
	double	perp_wall_dist;
	int		line_height;
	int		line_height_cubic;
	t_ivec2	unclamped_line_height;
	t_ivec2	coord1;
	t_ivec2	coord2;
	int		is_see_through;
}	t_ray;

typedef struct s_row
{
	t_vec2	ray_dir;
	t_vec2	ray_dir_step;
	double	pos_z;
	double	row_dist;
	t_vec2	floor_step;
	t_vec2	floor;
}	t_row;

typedef struct s_cell
{
	int		is_obstacle;
	int		is_door;
	int		is_goal;
	int		is_indoors;
	double	height;
	t_img	*tex_floor;
	t_img	*tex_ceiling;
	t_img	*tex_north;
	t_img	*tex_east;
	t_img	*tex_south;
	t_img	*tex_west;
}	t_cell;

typedef struct s_spr
{
	t_vec2	pos;
	t_img	*img;
}	t_spr;

typedef struct s_map
{
	t_ivec2	size;
	t_vec2	start_pos;
	t_vec2	start_dir;
	double	dof;
	double	fog_width;
	t_color	fog_color;
	t_img	*skybox;
	t_png	*background;
	t_cell	*cells;
	t_spr	*sprites;
	t_ivec2	minimap_offset;
	t_ivec2	minimap_center;
	int		minimap_radius;
	int		minimap_zoom;
	int		minimap_cell_amount;
}	t_map;

typedef struct s_player
{
	double	radius;
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

typedef struct s_man
{
	GLuint		shader_program;
	GLint		uniform_loc;
	double		dt;
	long		dt_ms;
	int			fps;
	t_res		res;
	t_frame		*frame[NBR_FRAMES + 1];
	int			curr_frame;
	t_player	player;
	double		move_speed;
	double		rotate_speed;
	t_ivec2		move_action;
	int			rotate_action;
	int			l_click_action;
	int			r_click_action;
	t_ivec2		cursor;
	t_img		images[NBR_IMG];
	t_map		*map;
}	t_man;

extern t_man	g_man;

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
void		draw_font_default(t_man *man, t_frame *frame, t_ivec2 *pos,
				char *str);
void		draw_image(t_frame *frame, t_img *img, t_ivec2 pos);
void		draw_cursor(t_frame *frame, t_img *img, t_ivec2 p, int cyc);
t_img		*get_image(t_man *man, const char *id);
void		advance_all_image_cycles(t_man *man);

/* Fog ---------------------------------------------------------------------- */

void		update_dof(t_map *m, double increment);
double		get_fog_width(double dof);
void		apply_wall_shadow(t_color *wall, t_color c, int y, t_ivec2 height);
void		apply_corner_shadow(t_color *wall, t_color c, int img_coord_x,
				int img_size_x);
void		apply_wall_fog(t_color *wall, t_color c, double dist, double dof);
void		update_background(t_map *m);

/* Files -------------------------------------------------------------------- */

char		*read_file(const char *filepath);
char		*gnl(int fd);
char		*strjoin(char const *s1, char const *s2);
char		*itoa_dec(int number);

/* JSON --------------------------------------------------------------------- */

char		**get_json_content(const char *filepath);
void		free_json_content(char **content);
int			set_png_file_obj(t_png *file, char **lines);
void		free_and_reset_png_file_obj(t_png *file);
int			is_field(char *line, const char *field);
char		*get_string_value(char *line);
int			get_int_value(char *line);
t_ivec2		get_ivec2_value(char *line);
void		parse_segments(t_png *file, char **lines, size_t *i);

/* Images ------------------------------------------------------------------- */

t_png		*load_png_from_path(const char *path);
t_png		*create_empty_png(t_ivec2 size);
void		compose_skybox(t_map *m, t_img *src, t_color fog);
void		compose_background(t_man *man);
void		draw_background(t_man *man, t_frame *f);
void		apply_vertical_gradient(t_png *img, t_color color);
void		free_png(t_png *png);
void		add_outline_to_font(t_img *font);
int			create_images_from_file(t_man *man, t_png *file, size_t *i_img);
void		free_images(t_man *man);
void		free_image(t_img *img);
int			set_image_array(t_man *man, const char *path);
int			calculate_image_average_color(t_img *img);
t_img		*duplicate_image(const char *dst_id, t_img *src);

/* Game --------------------------------------------------------------------- */

void		set_dt_and_fps(t_man *man);
void		display_fps(t_man *man, t_frame *f, t_ivec2 pos);
void		run_game_loop(t_man *man);
void		door_routine(t_man *man);
void		reset_global_coordinates(void);
void		update_global_coordinates(void);

/* Raycasting --------------------------------------------------------------- */

void		raycasting(t_man *man, t_frame *f);
void		perform_dda(t_man *man, double cam_x, t_list **list);
int			dda_add_to_list(t_map *m, t_ray *r, double *biggest_height);
void		cast_floor(t_man *man, t_frame *f);
void		cast_ceiling_x(t_man *man, t_frame *f, double *z_buffer, int x);
void		draw_wall(t_man *man, t_frame *f, t_ray *r);
int			is_corner(t_map *m, t_ray *r, int img_coord_x, int img_size_x);
void		sort_sprites(t_man *man, int *spr_order, double *spr_dist,
				int spr_amount);
void		cast_sprites(t_man *man, t_frame *f, double *z_buffer,
				int *spr_order, double *spr_dist, int x);

/* Input -------------------------------------------------------------------- */

void		physical_key_callback(GLFWwindow *window, int key, int scancode,
				int action, int mods);
void		scroll_callback(GLFWwindow *window, double x_offset,
				double y_offset);
void		mouse_callback(GLFWwindow *window, int button, int action,
				int mods);
void		cursor_pos_callback(GLFWwindow *window, double xpos, double ypos);

/* Maps --------------------------------------------------------------------- */

int			create_map(t_man *man);
void		free_map(t_man *man);
void		draw_minimap(t_man *man, t_frame *f);
void		decrease_minimap_zoom(t_man *man);
void		increase_minimap_zoom(t_man *man);

/* Mesh --------------------------------------------------------------------- */

int			create_mesh(void);
void		render_mesh(void);
void		free_mesh(void);

/* Transform ---------------------------------------------------------------- */

void		reset_player_transform(t_man *man);
void		update_player_transform(t_man *man);
void		rotate_player(t_man *man, double angle);
void		echolocation(t_man *man, int has_player_moved);

/* Shader Program ----------------------------------------------------------- */

int			create_shader_program(void);
void		free_shader_program(t_man *man);

/* Frames ------------------------------------------------------------------- */

int			create_frames(t_man *man);
void		use_frame(t_frame *f);
void		clear_drawing(t_frame *f);
void		save_drawing(t_frame *f);
void		free_frames(t_man *man);

/* Uniform ------------------------------------------------------------------ */

int			create_uniform(t_man *man);
void		free_uniform(t_man *man);

/* Windowing ---------------------------------------------------------------- */

GLFWwindow	*get_window(t_man *man, const char *title);
void		toggle_fullscreen(t_man *man, GLFWwindow *window);

#endif
