#ifndef OLAFUR_H
# define OLAFUR_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <ctype.h>
# include <limits.h>
# include <float.h>
# include <fcntl.h>
# include <unistd.h>
# include <dirent.h>
# include <pthread.h>
# include <sys/time.h>
# include <sys/stat.h>
# include <math.h>
# include "math_extra.h"
# include "lib/lodepng.h"
# include "lib/al.h"
# include "lib/alc.h"
# include "lib/dr_mp3.h"
# include <GL/gl.h>
# include "lib/glfw3.h"
# include "gl_functions.h"
# include "macros.h"
# include "structs.h"

/* Window ------------------------------------------------------------------- */

int			create_window(t_man *man, const char *title, t_ivec2 size,
				float aspect_ratio);
void		set_monitor_size(t_man *man);
int			set_resolution(t_man *man, t_ivec2 window_size,
				float aspect_ratio);
void		set_viewport(t_man *man, t_ivec2 window_size);
void		set_vsync(t_man *man);
void		toggle_fullscreen(t_man *man);
void		framebuffer_size_callback(GLFWwindow *window, int x, int y);
void		close_window_callback(t_man *man);
int			window_pos_to_view(t_man *man, double xpos, double ypos,
				t_ivec2 *pos);

/* Frame -------------------------------------------------------------------- */

int			init_frame(t_man *man);
void		display_frame(t_man *man);
void		free_frame(t_man *man);
int			create_shader_program(void);
void		free_shader_program(t_man *man);
int			create_uniform(t_man *man);
void		free_uniform(t_man *man);
int			create_mesh(void);
void		render_mesh(void);
void		free_mesh(void);

/* Input -------------------------------------------------------------------- */

void		physical_key_callback(GLFWwindow *window, int key, int scancode,
				int action, int mods);
void		scroll_callback(GLFWwindow *window, double x_offset,
				double y_offset);
void		mouse_callback(GLFWwindow *window, int button, int action,
				int mods);
void		cursor_pos_callback(GLFWwindow *window, double xpos, double ypos);
void		init_input_handling(t_man *man);
void		init_input_keys(t_man *man);
void		execute_input_action(t_man *man, int keycode, t_key_state state);
void		cancel_keys_in_game_state(t_man *man);
void		change_game_state(t_man *man, t_game_state new_state);
void		action_move_left(t_man *man, int set);
void		action_move_right(t_man *man, int set);
void		action_move_to_start(t_man *man, int set);
void		action_toggle_vsync(t_man *man, int set);
void		action_toggle_fullscreen(t_man *man, int set);
void		action_toggle_debug(t_man *man, int set);
void		action_close_window(t_man *man, int set);
void		teleport_through_key_sequence(t_man *man, int key);
void		action_move_forward(t_man *man, int set);
void		action_move_backward(t_man *man, int set);
void		action_turn_left(t_man *man, int set);
void		action_turn_right(t_man *man, int set);
void		action_run(t_man *man, int set);

/* Time --------------------------------------------------------------------- */

void		set_dt_and_fps(t_man *man);
void		display_fps(t_man *man);

/* Colors ------------------------------------------------------------------- */

t_color		get_color_rgba(t_ubyte r, t_ubyte g, t_ubyte b, t_ubyte a);
t_color		alpha_blending(t_color prev, t_color new);
t_color		get_color_hex(const char *str, t_ubyte alpha);

/* Draw --------------------------------------------------------------------- */

void		draw_point(t_man *man, t_color c, int x, int y);
void		draw_scaled_point(t_man *man, t_color c, t_ivec2 at, int scale);
void		fill_frame(t_man *man, t_color c);
void		draw_image(t_man *man, t_img *img, t_ivec2 pos);
void		draw_image_scaled(t_man *man, t_img *img, t_ivec2 pos, int scale);
void		draw_cursor(t_man *man, t_img *img, t_ivec2 p, int cyc);
void		draw_font(t_man *man, t_ivec2 *pos, const char *str);
void		draw_pixel(t_color *buf, t_color c, t_ivec2 coord, t_ivec2 size);
void		draw_point_fast(t_man *man, t_color c, int x, int y);
void		draw_line(t_man *man, t_vert v1, t_vert v2);
void		draw_rectangle(t_man *man, t_vert v, t_ivec2 size);
void		draw_rectangle_full(t_man *man, t_vert v, t_ivec2 size);
void		draw_circle(t_man *man, t_vert center, int radius);
void		draw_circle_full(t_man *man, t_vert center, int radius);
void		draw_circle_full_gradient(t_man *man, t_vert center, int radius,
				t_color edge);
void		draw_shape(t_man *man, t_vert arr[], int len);
void		draw_shape_full(t_man *man, t_vert arr[], int len);
void		fill_polygon(t_man *man, t_vert arr[], int len);

/* Images ------------------------------------------------------------------- */

t_png		*load_png_from_path(const char *path);
void		free_png(t_png *png);
t_img		*create_empty_image(const char *id, t_ivec2 size, t_ubyte alpha);
t_img		*duplicate_image(const char *dst_id, t_img *src);
int			update_image_array(t_man *man, const char *path);
int			register_json_path(t_man *man, const char *path);
void		discard_new_images(t_man *man, size_t start, const char *path);
int			check_png_segments(t_png *file);
int			create_images_from_file(t_man *man, t_png *file, size_t *i_img);
void		cut_image(t_png *file, t_img *img, size_t i);
t_img		*get_image(t_man *man, const char *id);
void		advance_all_image_cycles(t_man *man);
void		add_outline_to_font(t_img *font);
void		free_images(t_man *man);
void		free_image(t_img *img, void (free_fct)(void *));
t_png		*create_empty_png(t_ivec2 size, t_ubyte alpha);
t_img		*add_image(t_man *man, const char *path);
int			calculate_image_average_color(t_img *img);
void		compose_skybox(t_man *man, t_map *map, t_img *src);
void		compose_background(t_man *man, t_map *map);
void		add_fog_overlay(t_png *png, float fog_width, t_color fog);
void		select_background_frame(t_map *map);
void		draw_background(t_man *man);

/* JSON --------------------------------------------------------------------- */

int			parse_json_images(t_json *j, const char *path,
				const char *abs_path);
void		free_json_images(t_json *j);
int			json_parse_segments(t_json *j, t_png *file);
char		json_peek(t_json *j);
int			json_expect(t_json *j, char c, const char *msg);
int			json_error(t_json *j, const char *msg);
int			json_list(t_json *j, char close, t_json_item item, void *ctx);
int			json_key(t_json *j, char **key);
int			json_string(t_json *j, char **dst);
int			json_int(t_json *j, int *dst);
int			json_ivec2(t_json *j, t_ivec2 *dst);
int			json_unknown_field(t_json *j, const char *key);
int			json_skip_value(t_json *j);
void		free_and_reset_png_file_obj(t_png *file);
void		free_segments(t_png *file);

/* Files -------------------------------------------------------------------- */

char		*get_absolute_path(const char *path);
const char	*get_filename(const char *path);
int			has_extension(const char *name, const char *ext);
char		**read_file_lines(const char *filepath);
void		**extract_arr_elems(void **array, size_t from, size_t to);
void		remove_arr_elems(void **array, size_t from, size_t to,
				void (*free_element)(void *));

/* Utils -------------------------------------------------------------------- */

int			put_error(const char *msg, const char *arg, int errnum);
char		*gnl(int fd);
char		*strjoin(char const *s1, char const *s2);
char		*itoa_dec(int number);
void		free_arr(void **arr, void (free_fct)(void *));
size_t		count_arr_elems(void **arr);
t_list		*list_new(void *data);
void		list_del_one(t_list **list, void (*del)(void *));
void		list_clear(t_list **list, void (*del)(void *));
char		**split(const char *s, char c);
char		*get_substr(const char *s, unsigned int start, size_t len);
t_list		*list_last(t_list *list);
void		list_add_back(t_list **list, t_list *new);
int			push_strlist(t_strlist *list, const char *s);
void		sort_strlist(t_strlist *list);
void		free_strlist(t_strlist *list);

/* Init --------------------------------------------------------------------- */

void		deinit(t_man *man);
char		*find_first_map_path(const char *dir_path);
void		run_game_loop(t_man *man);
void		game_loop(t_man *man);
void		update_mouse_pos(t_man *man);

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
int			process_doors(t_man *man, t_map *map);
int			process_portals(t_man *man, t_map *map);
int			process_sprites(t_man *man, t_map *map);
int			process_teleport(t_map *map);
int			create_audio_sources_for_sprites(t_map *map);
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
float		get_num_flt(const char *arg);

/* Maps --------------------------------------------------------------------- */

int			add_map(t_man *man, const char *filepath);
int			add_map_and_submaps(t_man *man, const char *filepath);
int			add_map_and_submaps_recursive(t_man *man, const char *filepath);
void		free_sprite_array(t_map *map);
void		free_maps(t_man *man);
void		free_map(t_map *map);
void		draw_minimap(t_man *man, t_map *map);
void		draw_rect_minimap(t_man *man, t_vert v);
void		init_minimap_values(t_man *man);
void		decrease_minimap_zoom(t_man *man);
void		increase_minimap_zoom(t_man *man);
t_vec2		get_dir_from_cardinal(char c);
char		get_card_from_str(const char *s);
char		get_offset_from_str(const char *s);

/* Game --------------------------------------------------------------------- */

void		display_game_gui(t_man *man, t_map *map);
void		door_routine(t_man *man);
t_door		*get_door(t_map *m, int x, int y);
int			is_player_next_to_door_face(t_door *d, t_ivec2 p);
void		trigger_door(t_door *d, int is_open);
void		portal_routine(t_man *man);
t_portal	*get_portal(t_man *man);
void		open_and_close_portals(t_man *man);
int			is_portal_visible(t_portal *portal);
t_portal	*get_portal_at(t_map *m, int x, int y);
int			is_player_next_to_portal_face(t_portal *pt, t_ivec2 p);
void		trigger_portal(t_portal *p, int is_open);

/* Transform ---------------------------------------------------------------- */

void		reset_player(t_man *man);
void		reset_player_transform(t_man *man);
void		set_player_transform(t_man *man, t_vec2 new_pos, t_vec2 new_dir);
void		update_player_transform(t_man *man);
void		prevent_out_of_bounds(t_man *man);
void		handle_player_speed(t_man *man, int shift_pressed);
void		rotate_player(t_man *man, float angle);
void		cross_goal_if_unlocked(t_man *man, t_map *m);
void		disable_collision_with_dst_portal_if_within(t_man *man, t_map *m,
				t_vec2 pos);
int			push_back_on_collision(t_man *man, t_map *m, t_vec2 pos,
				t_vec2 delta);
t_cell		*get_cell(t_map *m, int x, int y);
void		unstuck_from_wall(t_man *man, t_map *m);
void		adjust_position_on_sprite_collision(t_man *man, t_map *map);

/* Raycasting --------------------------------------------------------------- */

void		raycasting(t_man *man);
void		perform_dda(t_man *man, float cam_x);
void		init_ray_data_x(t_ray *r);
void		init_ray_data_y(t_ray *r);
void		update_ray_data(t_ray *r);
int			dda_add_to_list(t_man *man, t_ray **r, float *max_height);
t_cell		*resolve_portal_rendering(t_man *man, t_ray **r, t_cell *cell);
void		set_texture_and_is_see_through(t_ray *r, t_cell *c);
void		cast_floor_and_ceiling(t_man *man);
void		draw_fc_pixel(t_man *man, t_row *row, t_ivec2 at, t_map *map);
void		fill_sky_pair(t_man *man, t_map *map, int y);
void		draw_wall(t_man *man, t_ray *r, t_img *tex);
int			calculate_tex_coord_x(t_img *tex, t_ray *r);
float		calculate_initial_tex_pos(t_man *man, t_ray *r, t_img *tex);
t_ivec2		calculate_texel_range(t_ray *r, t_img *tex);
int			is_corner(t_map *m, t_ray *r, int img_coord_x, int img_size_x);
void		sort_sprites_by_dist(t_man *man);
void		cast_sprites(t_man *man, int x, float near_bound);
void		render_sprite_column(t_man *man, t_spr *s, int x);

/* Fog ---------------------------------------------------------------------- */

void		init_fog(t_man *man);
float		get_fog_width(float dof);
int			wall_shadow_factor8(t_ray *r, int y);
void		set_wall_shadow_params(t_ray *r);
float		corner_intensity_of(t_ray *r, t_img *tex, int tex_x);
float		fog_factor_of(float dist, float dof);
void		apply_fog_factor(t_color *wall, t_color c, int factor8);

/* Audio -------------------------------------------------------------------- */

void		audio_init(t_audio *a);
void		audio_deinit(t_audio *a);
void		audio_switch_map_sounds(t_man *man);
void		audio_listener_transform(t_player *p);
t_a_track	*audio_track_create(t_audio *a, const char *mp3_filename,
				int force_mono);
int			load_mp3(const char *filename, t_a_track *t, int force_mono);
int			audio_track_create_chunks(t_a_track *t);
void		audio_track_delete(t_audio *a, t_a_track **track);
void		audio_track_delete_all(t_audio *a);
t_a_source	*audio_source_create(t_a_track *t, int is_loop);
void		audio_source_spatialization(t_a_source *s, int enable);
void		audio_source_pos(t_a_source *s, t_vec2 pos);
void		audio_source_change_track(t_a_source *s, t_a_track *t);
void		audio_source_unset_track(t_a_source *s);
void		audio_source_delete(void *data);
void		audio_source_delete_marked(t_map *map);
void		audio_source_delete_map(t_map *map);
void		audio_source_play(t_a_source *s);
void		audio_source_end_loop_thread(t_a_source *s);
void		audio_source_pause(t_a_source *s);
void		audio_source_stop(t_a_source *s);

#endif
