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
# define SPRITE_LEN 6
# define DEFAULT_MOVE_SPEED 2.0
# define DEFAULT_ROT_SPEED 0.25

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
	GLubyte	r;
	GLubyte	g;
	GLubyte	b;
	GLubyte	a;
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
	int		cycle_len;
	int		cycle_index;
	long	elapsed_time_in_ms;
	long	cycle_time_in_ms;
	t_color	**cycle;
	t_color	**cycle_shadow;
}	t_spr;

typedef struct s_img_seg
{
	char	*id;
	t_ivec2	size;
	t_ivec2	shadow_offset;
	int		still_frame;
	int		cycle_len;
	long	cycle_time_in_ms;
	t_ivec2	*cycle;
}	t_img_seg;

typedef struct s_img
{
	char		*path;
	char		*path_shadow;
	t_ivec2		size;
	int			is_see_through;
	t_color		average_color;
	GLubyte		*buf;
	GLubyte		*buf_shadow;
	int			segment_len;
	t_img_seg	*seg;
}	t_img;

typedef struct s_ray
{
	t_vec2	ray_dir;
	t_ivec2	m_index;
	int		side;
	double	perp_wall_dist;
	int		line_height;
	t_ivec2	coord1;
	t_ivec2	coord2;
	t_list	*alpha;
}	t_ray;

typedef struct s_cell
{
	int		is_obstacle;
	int		is_door;
	t_img	*tex_floor;
	t_img	*tex_ceiling;
	t_img	*tex_north;
	t_img	*tex_east;
	t_img	*tex_south;
	t_img	*tex_west;
}	t_cell;

typedef struct s_map
{
	t_ivec2	size;
	t_vec2	start_pos;
	t_vec2	start_dir;
	double	dof;
	double	fog_width;
	t_color	fog_color;
	t_img	*skybox;
	int		img_len;
	t_img	**img;
	t_spr	sprites[SPRITE_LEN];
	t_cell	*cells;
}	t_map;

typedef struct s_player
{
	t_vec2	pos;
	t_vec2	dir;
	t_vec2	plane;
	int		height;
}	t_player;

typedef struct s_frame
{
	GLuint	id;
	t_ivec2	size;
	t_ivec2	real_size;
	int		thickness;
	GLubyte	*buf;
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
	int			fps;
	t_res		res;
	t_frame		*frame[NBR_FRAMES + 1];
	int			curr_frame;
	double		tex_in_dof;
	t_player	player;
	double		movement_speed;
	double		rotation_speed;
	t_ivec2		movement_action;
	int			rotation_action;
	int			click_action;
	t_ivec2		cursor;
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

t_color		get_color_rgba(GLubyte r, GLubyte g, GLubyte b, GLubyte a);
t_color		get_color_hex(const char *str, GLubyte alpha);
t_color		get_alpha_blended_color(t_color prev, t_color new);
int			cmp_color(t_color a, t_color b);
t_color		calculate_average_color(t_img *img);
t_color		get_frame_color(t_frame *f, int x, int y);

/* Vectors ------------------------------------------------------------------ */

void		set_vec2(t_vec2 *v, double x, double y);
void		set_ivec2(t_ivec2 *v, int x, int y);

/* Draw --------------------------------------------------------------------- */

void		draw_point(t_frame *f, t_color color, int x, int y);
void		draw_point_img(t_img *img, t_color c, int x, int y);
void		draw_line(t_frame *f, t_vert v1, t_vert v2);
void		draw_rectangle(t_frame *f, t_vert v, t_ivec2 size);
void		draw_rectangle_full(t_frame *f, t_vert v, t_ivec2 size);
void		draw_circle(t_frame *f, t_vert center, int radius);
void		draw_circle_full(t_frame *f, t_vert center, int radius);
void		draw_circle_full_grad(t_frame *f, t_vert center, int radius,
				t_color edge);
void		draw_shape(t_frame *f, t_vert arr[], int len);
void		draw_shape_full(t_frame *f, t_vert arr[], int len);
void		draw_image(t_frame *f, t_img *img);
void		draw_image_with_x_offset(t_frame *f, t_img *img, int x_offset);
void		draw_font_default(t_frame *frame, t_ivec2 *pos, char *str);
void		draw_sprite(t_frame *frame, t_spr *sprite, t_ivec2 pos, long dt_ms);
t_spr		*get_sprite(char *id);

/* Fog ---------------------------------------------------------------------- */

void		update_dof(t_map *m, double increment);
double		get_fog_width(double dof);
void		apply_wall_fog(t_color *wall, t_color fog, double dist, double dof);
void		update_background(t_map *m, t_img *bg);

/* Files -------------------------------------------------------------------- */

char		*read_file(const char *filepath);
char		*gnl(int fd);
char		*strjoin(char const *s1, char const *s2);

/* JSON --------------------------------------------------------------------- */

char		**get_json_content(const char *filepath);
void		free_json_content(char **content);
int			is_field(char *line, const char *field);
char		*get_string_value(char *line);
int			get_int_value(char *line);
t_ivec2		get_ivec2_value(char *line);

/* Images ------------------------------------------------------------------- */

t_img		*load_image_from_file(const char *png_path);
t_img		*create_image(t_color c, t_ivec2 size);
t_img		*compose_skybox(const char *path_skybox, t_color fog);
t_img		*compose_background(t_map *m);
void		draw_background(t_frame *f, t_map *m);
void		apply_vertical_gradient(t_img *img, t_color color);
void		free_image(t_img *s);
void		add_outline_to_font(t_spr *font);
int			set_img_file_obj(t_img *file, char **lines, size_t *i);
int			create_sprites_from_file(t_img *file, int *i_spr);
void		free_sprites(void);
int			set_sprite_array(char *path);

/* Game --------------------------------------------------------------------- */

void		set_dt_and_fps(void);
void		run_game_loop(t_map *m);
void		door_routine(t_map *m);
void		raycasting(t_map *m);
void		draw_wall(t_map *m, t_frame *f, t_ray *r);
void		reset_global_coordinates(void);
void		update_global_coordinates(void);

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
void		set_minimap_display(int remove_from_factor);
void		draw_minimap(t_map *map);
void		free_maps(void);

/* Mesh --------------------------------------------------------------------- */

int			create_mesh(void);
void		render_mesh(void);
void		free_mesh(void);

/* Player ------------------------------------------------------------------- */

void		reset_player_transform(t_map *m);
void		update_player_transform(t_map *m);
double		get_angle_from_dir(t_vec2 dir);

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
