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
# include <math.h>
# include "math_extra.h"
# include "lodepng.h"
# ifdef GL
#  include <GL/gl.h>
#  include "glfw3.h"
#  include "gl_functions.h"
# else
#  include "mlx.h"
# endif
# include "macros.h"
# include "structs.h"

/* Colors ------------------------------------------------------------------- */

t_color		get_color_rgba(t_ubyte r, t_ubyte g, t_ubyte b, t_ubyte a);
t_color		get_color_hex(const char *str, t_ubyte alpha);
t_color		get_alpha_blended_color(t_color prev, t_color new);

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
void		draw_png_with_x_offset(t_frame *f, t_png *png, int x_offset);
void		fill_frame(t_frame *f, t_color c);
void		draw_cursor(t_frame *frame, t_img *img, t_ivec2 p, int cyc);
t_img		*get_image(t_man *man, const char *id);
void		advance_all_image_cycles(t_man *man);
t_color		get_frame_pixel(t_frame *f, int x, int y);
void		set_frame_pixel(t_frame *f, t_color c, int x, int y);

/* Fog ---------------------------------------------------------------------- */

void		init_fog(t_man *man);
void		update_dof(t_man *man, double increment);
double		get_fog_width(double dof);
void		apply_wall_shadow(t_color *wall, t_color c, int y, t_ivec2 height);
void		apply_corner_shadow(t_color *wall, t_color c, int img_coord_x,
				int img_size_x);
void		apply_wall_fog(t_color *wall, t_color c, double dist, double dof);

/* Files -------------------------------------------------------------------- */

char		*read_file(const char *filepath);
char		**read_file_lines(const char *filepath);
void		free_arr(void **arr, void (free_fct)(void *));
size_t		count_arr_elems(void **arr);

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
t_img		*create_empty_image(const char *id, t_ivec2 size);
t_img		*add_new_image(t_man *man, const char *path);
void		compose_skybox(t_man *man, t_map *map, t_img *src);
void		compose_background(t_man *man, t_map *map);
void		draw_background(t_man *man, t_frame *f);
void		free_png(t_png *png);
int			set_image_array(t_man *man, const char *path);
int			create_images_from_file(t_man *man, t_png *file, size_t *i_img);
void		cut_image(t_png *file, t_img *img, size_t i);
void		free_images(t_man *man);
void		free_image(t_img *img, void (free_fct)(void *));
int			calculate_image_average_color(t_img *img);
void		add_outline_to_font(t_img *font);
t_img		*duplicate_image(const char *dst_id, t_img *src);

/* Game --------------------------------------------------------------------- */

void		set_dt_and_fps(t_man *man);
void		display_fps(t_man *man, t_frame *f);
void		display_game_gui(t_man *man, t_frame *f);
void		display_game_over_screen(t_man *man, t_frame *f);
void		door_routine(t_man *man);
void		portal_routine(t_man *man);
t_portal	*get_portal(t_man *man);

/* Raycasting --------------------------------------------------------------- */

void		raycasting(t_man *man, t_frame *f);
void		perform_dda(t_man *man, double cam_x, t_list **list);
int			dda_add_to_list(t_man *man, t_map *m, t_ray *r, double *max_height);
void		cast_floor(t_man *man, t_frame *f);
void		cast_ceiling_x(t_man *man, t_frame *f, int x);
void		draw_wall(t_man *man, t_frame *f, t_ray *r, int is_portal);
int			is_corner(t_map *m, t_ray *r, int img_coord_x, int img_size_x);
void		sort_sprites_by_dist(t_man *man);
void		swap_elements(void **a, void **b);
void		cast_sprites(t_man *man, int x);

/* Maps --------------------------------------------------------------------- */

int			find_map_index(t_man *man, const char *filepath);
int			add_map(t_man *man, const char *filepath);
t_map		*create_map(t_man *man, const char *filepath);
int			extract_maps_and_player_start(t_map *m);
int			extract_sprites(t_man *man, t_map *map);
int			create_portal_array(t_man *man, t_map *map);
t_portal	*find_portal(t_map *map, int x, int y);
void		free_sprite_array(t_map *map);
void		free_maps(t_man *man);
void		free_map(t_map *map);
void		free_cells(t_map *map);
void		draw_minimap(t_man *man, t_frame *f, t_map *map);
void		init_minimap_values(t_man *man);
void		decrease_minimap_zoom(t_man *man);
void		increase_minimap_zoom(t_man *man);
t_vec2		get_dir_from_cardinal(char c);
char		opp_cardinal(char cardinal);

/* Transform ---------------------------------------------------------------- */

void		reset_player_transform(t_man *man);
void		update_player_transform(t_man *man);
void		prevent_out_of_bounds(t_man *man);
void		handle_player_speed(t_man *man, int shift_pressed);
void		rotate_player(t_man *man, double angle);
void		echolocation(t_man *man, int has_player_moved);
void		cross_goal_if_unlocked(t_man *man);
void		disable_collision_with_dst_portal_if_within(t_man *man, t_map *m,
				t_vec2 pos);
int			push_back_on_collision(t_man *man, t_map *m, t_vec2 pos,
				t_vec2 delta);
void		unstuck_from_wall(t_man *man, t_map *m);
int			collect_sprite(t_man *man, int sprite_index);

/* Frames ------------------------------------------------------------------- */

int			init_frames(t_man *man);
void		clear_frame(t_frame *f);
void		display_frame(t_man *man, t_frame *f);
void		free_frames(t_man *man);

/* Windowing ---------------------------------------------------------------- */

void		set_resolution(t_man *man, t_ivec2 monitor_size, int width,
				int height);
void		set_viewport(t_man *man, t_ivec2 framebuffer_size);

/* Input -------------------------------------------------------------------- */

void		execute_input_action(t_man *man, int key, t_key_state new_state);
void		init_keys_wasdqe(t_man *man);
void		init_keys_arrows(t_man *man);
void		init_keys_misc(t_man *man);
int			key_press_callback(int keycode, t_man *man);
int			key_release_callback(int keycode, t_man *man);
int			mouse_press_callback(int button, int x, int y, t_man *man);
int			mouse_release_callback(int button, int x, int y, t_man *man);
int			mouse_move_callback(int x, int y, t_man *man);
int			focus_loss_callback(t_man *man);
int			cross_window_button_callback(t_man *man);
void		action_move_x_incr(t_man *man);
void		action_move_x_decr(t_man *man);
void		action_move_y_incr(t_man *man);
void		action_move_y_decr(t_man *man);
void		action_rotate_incr(t_man *man);
void		action_rotate_decr(t_man *man);
void		action_speed_walk(t_man *man);
void		action_speed_run(t_man *man);
void		action_toggle_fullscreen(t_man *man);
void		action_toggle_debug(t_man *man);
void		action_close_window(t_man *man);
void		poll_input_events(t_man *man);

/* Init --------------------------------------------------------------------- */

int			create_window(t_man *man, const char *title, int width, int height);
void		init_input_handling(t_man *man);
void		run_game_loop(t_man *man);
int			game_loop(t_man *man);
void		update_mouse_pos(t_man *man);
void		toggle_fullscreen(t_man *man);

/* End ---------------------------------------------------------------------- */

void		deinit(t_man *man);
int			put_error(t_man *man, char *msg, int errno);

/* Utils -------------------------------------------------------------------- */

char		*itoa(int number);
char		**split(const char *s, char c);
char		*strjoin(char const *s1, char const *s2);
char		*strjoin_arr(char **arr, size_t start, size_t end);
char		*gnl(int fd);
t_list		*list_last(t_list *list);
t_list		*list_new(void *data);
void		list_add_front(t_list **list, t_list *new);
void		list_add_back(t_list **list, t_list *new);
void		list_del_one(t_list **list, void (*del)(void*));

#endif
