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
# include <sys/stat.h>
# include <pthread.h>
# include <math.h>
# include "math_extra.h"
// IMAGE ------------
# include "lib/lodepng.h"
// AUDIO ------------
# include "lib/al.h"
# include "lib/alc.h"
# include "lib/dr_mp3.h"
// ------------------
# ifdef GL
#  include <GL/gl.h>
#  include "lib/glfw3.h"
#  include "gl_functions.h"
# else
#  include "sua.h"
# endif
# include "macros.h"
# include "structs.h"

/* Colors ------------------------------------------------------------------- */

t_color		get_color_rgba(t_ubyte r, t_ubyte g, t_ubyte b, t_ubyte a);
t_color		get_color_hex(const char *str, t_ubyte alpha);
t_color		alpha_blending(t_color prev, t_color new);

/* Draw --------------------------------------------------------------------- */

void		draw_pixel(t_color *buf, t_color c, t_ivec2 coord, t_ivec2 size);
void		draw_point(t_man *man, t_color color, int x, int y);
void		draw_line(t_man *man, t_vert v1, t_vert v2);
void		draw_rectangle(t_man *man, t_vert v, t_ivec2 size);
void		draw_rectangle_full(t_man *man, t_vert v, t_ivec2 size);
void		draw_circle(t_man *man, t_vert center, int radius);
void		draw_circle_full(t_man *man, t_vert center, int radius);
void		draw_circle_full_gradient(t_man *man, t_vert center, int radius,
				t_color edge);
void		draw_shape(t_man *man, t_vert arr[], int len);
void		draw_shape_full(t_man *man, t_vert arr[], int len);
void		draw_font_default(t_man *man, t_ivec2 *pos, const char *str);
void		draw_image(t_man *man, t_img *img, t_ivec2 pos);
void		draw_png_with_x_offset(t_man *man, t_png *png, int x_offset);
void		fill_frame(t_man *man, t_color c);
void		draw_cursor(t_man *man, t_img *img, t_ivec2 p, int cyc);
t_img		*get_image(t_man *man, const char *id);
void		advance_all_image_cycles(t_man *man);

/* Fog ---------------------------------------------------------------------- */

void		init_fog(t_man *man);
void		update_dof(t_man *man, double increment);
double		get_fog_width(double dof);
void		apply_wall_shadow(t_color *wall, t_color c, int y, t_ivec2 height);
void		apply_corner_shadow(t_color *wall, t_color c, int img_coord_x,
				int img_size_x);
void		apply_wall_fog(t_color *wall, t_color c, double dist, double dof);

/* Files -------------------------------------------------------------------- */

char		*get_absolute_path(const char *path);
const char	*get_filename(const char *path);
char		*read_file(const char *filepath);
char		**read_file_lines(const char *filepath);
void		free_arr(void **arr, void (free_fct)(void *));
size_t		count_arr_elems(void **arr);
void		*extract_arr_elem(void **array, size_t index);
void		**extract_arr_elems(void **array, size_t from, size_t to);
void		remove_arr_elems(void **array, size_t from, size_t to,
				void (*free_element)(void *));

/* JSON --------------------------------------------------------------------- */

char		**get_json_content(t_man *man, const char *filepath);
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
t_png		*create_empty_png(t_ivec2 size, t_ubyte alpha);
t_img		*create_empty_image(const char *id, t_ivec2 size, t_ubyte alpha);
t_img		*add_image(t_man *man, const char *path);
void		compose_skybox(t_man *man, t_map *map, t_img *src);
void		compose_background(t_man *man, t_map *map);
void		draw_background(t_man *man);
void		free_png(t_png *png);
int			update_image_array(t_man *man, const char *path);
int			create_images_from_file(t_man *man, t_png *file, size_t *i_img);
void		cut_image(t_png *file, t_img *img, size_t i);
void		free_images(t_man *man);
void		free_image(t_img *img, void (free_fct)(void *));
int			calculate_image_average_color(t_img *img);
void		add_outline_to_font(t_img *font);
t_img		*duplicate_image(const char *dst_id, t_img *src);

/* Game --------------------------------------------------------------------- */

void		set_dt_and_fps(t_man *man);
void		display_fps(t_man *man);
void		display_game_gui(t_man *man, t_map *map);
void		door_routine(t_man *man);
void		portal_routine(t_man *man);
t_portal	*get_portal(t_man *man);
void		open_and_close_portals(t_man *man);
int			is_portal_visible(t_portal *portal);

/* Raycasting --------------------------------------------------------------- */

void		raycasting(t_man *man);
void		perform_dda(t_man *man, double cam_x);
void		init_ray_data_x(t_ray *r);
void		init_ray_data_y(t_ray *r);
void		update_ray_data(t_ray *r);
int			dda_add_to_list(t_man *man, t_ray **r, double *max_height);
t_cell		*resolve_portal_rendering(t_man *man, t_ray **r);
void		set_texture_and_is_see_through(t_ray *r, t_cell *c);
void		cast_floor(t_man *man);
void		cast_ceiling_x(t_man *man, int x);
void		draw_wall(t_man *man, t_ray *r, t_img *tex);
int			is_corner(t_map *m, t_ray *r, int img_coord_x, int img_size_x);
void		sort_sprites_by_dist(t_man *man);
void		swap_elements(void **a, void **b);
void		cast_sprites(t_man *man, int x);

/* Maps --------------------------------------------------------------------- */

int			add_map(t_man *man, const char *filepath);
void		free_sprite_array(t_map *map);
void		free_maps(t_man *man);
void		free_map(t_map *map);
void		draw_minimap(t_man *man, t_map *map);
void		draw_rect_minimap(t_man *man, t_vert v);
void		init_minimap_values(t_man *man);
void		decrease_minimap_zoom(t_man *man);
void		increase_minimap_zoom(t_man *man);
t_vec2		get_dir_from_cardinal(char c);
t_ivec2		get_dir_from_cardinal_ivec(char c);
char		opp_cardinal(char cardinal);
char		get_card_from_str(const char *s);

/* Transform ---------------------------------------------------------------- */

void		reset_player(t_man *man);
void		reset_player_transform(t_man *man);
void		set_player_transform(t_man *man, t_vec2 new_pos, t_vec2 new_dir);
void		update_player_transform(t_man *man);
void		prevent_out_of_bounds(t_man *man);
void		handle_player_speed(t_man *man, int shift_pressed);
void		rotate_player(t_man *man, double angle);
void		echolocation(t_man *man, int has_player_moved);
void		cross_goal_if_unlocked(t_man *man, t_map *m);
void		disable_collision_with_dst_portal_if_within(t_man *man, t_map *m,
				t_vec2 pos);
int			push_back_on_collision(t_man *man, t_map *m, t_vec2 pos,
				t_vec2 delta);
void		unstuck_from_wall(t_man *man, t_map *m);
void		adjust_position_on_sprite_collision(t_man *man, t_map *map);

/* Frame -------------------------------------------------------------------- */

int			init_frame(t_man *man);
void		display_frame(t_man *man);
void		free_frame(t_man *man);

/* Windowing ---------------------------------------------------------------- */

int			set_resolution(t_man *man, t_ivec2 window_size,
				double aspect_ratio);
void		set_viewport(t_man *man, t_ivec2 window_size);

/* Input -------------------------------------------------------------------- */

void		execute_input_action(t_man *man, int keycode, t_key_state state);
void		cancel_keys_in_game_state(t_man *man);
void		change_game_state(t_man *man, t_game_state new_state);
void		init_input_keys(t_man *man);
void		close_window_callback(t_man *man);
void		konami_code(t_man *man, int pressed_key);
void		action_move_right(t_man *man, int set);
void		action_move_left(t_man *man, int set);
void		action_move_forward(t_man *man, int set);
void		action_move_backward(t_man *man, int set);
void		action_turn_right(t_man *man, int set);
void		action_turn_left(t_man *man, int set);
void		action_run(t_man *man, int set);
void		action_toggle_fullscreen(t_man *man, int set);
void		action_toggle_debug(t_man *man, int set);
void		action_close_window(t_man *man, int set);
void		action_move_to_start(t_man *man, int set);

/* Audio -------------------------------------------------------------------- */

void		audio_init(t_audio *a);
void		audio_deinit(t_audio *a);
void		audio_switch_map_sounds(t_man *man);
t_a_track	*audio_track_create(t_audio *a, const char *mp3_filename,
				int force_mono);
int			load_mp3(const char *filename, t_a_track *t, int force_mono);
void		audio_track_delete(t_audio *a, t_a_track **track);
void		audio_track_delete_all(t_audio *a);
t_a_source	*audio_source_create(t_a_track *t, int is_loop);
void		audio_source_change_track(t_a_source *s, t_a_track *t);
void		audio_source_unset_track(t_a_source *s);
void		audio_source_delete(t_a_source **s);
void		audio_source_play(t_a_source *s);
void		audio_source_pause(t_a_source *s);
void		audio_source_stop(t_a_source *s);

/* Init --------------------------------------------------------------------- */

int			create_window(t_man *man, const char *title, t_ivec2 size,
				double aspect_ratio);
t_ivec2		get_monitor_size(t_man *man);
void		init_input_handling(t_man *man);
void		run_game_loop(t_man *man);
void		game_loop(t_man *man);
void		update_mouse_pos(t_man *man);
void		toggle_fullscreen(t_man *man);

/* End ---------------------------------------------------------------------- */

void		deinit(t_man *man);
int			put_error(t_man *man, const char *msg, const char *arg, int errno);

/* Utils -------------------------------------------------------------------- */

char		*itoa(int number);
char		**split(const char *s, char c);
char		*get_substr(const char *s, unsigned int start, size_t len);
char		*strjoin(char const *s1, char const *s2);
char		*strjoin_arr(char **arr, size_t start, size_t end);
char		*gnl(int fd);
t_list		*list_last(t_list *list);
t_list		*list_new(void *data);
void		list_add_front(t_list **list, t_list *new);
void		list_add_back(t_list **list, t_list *new);
void		list_del_one(t_list **list, void (*del)(void*));

/* Parsing ------------------------------------------------------------------ */

t_map		*create_map(t_man *man, const char *filepath);
t_map		*fetch_map_data(const char *filepath);
int			extract_maps(t_map *map);
int			process_img_assets(t_man *man, t_map *map);
int			process_sound_assets(t_man *man, t_map *map);
int			process_wall_types(t_man *man, t_map *map);
int			process_spec_wall_types(t_man *man, t_map *map);
int			process_background_image(t_man *man, t_map *map);
int			process_floor_types(t_man *man, t_map *map);
int			process_ceil_types(t_man *man, t_map *map);
int			process_portals(t_man *man, t_map *map);
int			process_sprites(t_man *man, t_map *map);
int			check_map_symbols(t_map *map);
int			make_maps_same_size(t_map *map);
int			set_player_data(t_map *map);
int			populate_cells(t_map *map);
int			set_special_cells(t_map *map, t_cell *c, t_ivec2 p);
void		release_parsing_data(t_map *map);
void		free_tex_w(char *tex[10][4]);
void		free_tex_fc(char *tex[10]);
void		remove_var_line(t_map *map, int i);
int			is_var_list_empty(t_map *map);
int			get_num_rgb(const char *arg);
int			get_num_int(const char *arg);
double		get_num_dbl(const char *arg);

#endif
