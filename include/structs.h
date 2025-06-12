#ifndef STRUCTS_H
# define STRUCTS_H

typedef unsigned char	t_ubyte;
typedef unsigned int	t_uint;
typedef struct s_man	t_man;
typedef struct s_map	t_map;

typedef enum e_game_state
{
	GAME_STATE_NONE,
	GAME_STATE_PLAY,
	GAME_STATE_SUCCESS,
	GAME_STATE_FAILURE
}	t_game_state;

typedef enum e_key_state
{
	KEY_STATE_NONE,
	KEY_STATE_PRESSED,
	KEY_STATE_HELD,
	KEY_STATE_RELEASED
}	t_key_state;

/* Each key has 3 potential game states for 3 potential key states */
typedef struct s_key
{
	int				keycode;
	t_key_state		state;
	void			(*actions[3][3])(t_man *man, int set);
}	t_key;

typedef struct s_list
{
	struct s_list	*prev;
	void			*data;
	struct s_list	*next;
}	t_list;

typedef struct s_a_track
{
	char	*name;
	double	seconds;
	ALvoid	*pcm_data;
	ALsizei	data_size;
	ALenum	format;
	ALsizei	freq;
	ALuint	*buffers;
	int		nbr_buffers;
}	t_a_track;

typedef struct s_a_source
{
	ALuint			source;
	t_a_track		*track;
	int				is_loop;
	pthread_t		loop_thread;
	int				is_looping;
	pthread_mutex_t	is_looping_mutex;
}	t_a_source;

typedef struct s_audio
{
	ALCdevice	*device;
	ALCcontext	*context;
	t_a_track	**tracks;
	t_a_source	*music;
}	t_audio;

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
	double	cam_x;
	t_vec2	origin_pos;
	t_vec2	origin_dir;
	t_vec2	origin_plane;
	t_map	*m;
	t_ivec2	m_index;
	t_vec2	ray_dir;
	t_vec2	delta_dist;
	t_ivec2	step;
	t_vec2	side_dist;
	int		side;
	double	perp_wall_dist;
	int		line_height;
	int		line_height_cubic;
	t_ivec2	unclamped_line_height;
	t_ivec2	coord1;
	t_ivec2	coord2;
	t_img	*tex;
	t_img	*tex_portal;
	int		is_see_through;
	int		invert_tex;
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

typedef struct s_door
{
	t_map		*m;
	t_ivec2		pos;
	char		cardinal;
	t_img		*tex_closed;
	t_img		*tex_open;
	int			is_plane;
	int			is_open;
}	t_door;

typedef struct s_portal
{
	int		preview;
	t_map	*src_map;
	int		is_corridor;
	char	src_cardinal;
	t_ivec2	src_pos;
	char	*path_dst_map;
	int		override_start_pos;
	char	dst_cardinal;
	t_ivec2	dst_pos;
	t_img	*tex_closed;
	t_img	*tex_open;
	int		is_open;
	int		is_visible;
	int		trigger_opposite;
}	t_portal;

typedef struct s_cell
{
	int			is_empty_space;
	int			is_visible;
	int			is_obstacle;
	int			is_goal;
	double		height;
	t_img		*tex_floor;
	t_img		*tex_ceiling;
	t_img		*tex_north;
	t_img		*tex_east;
	t_img		*tex_south;
	t_img		*tex_west;
	t_door		*door;
	t_portal	*portal;
}	t_cell;

typedef struct s_spr
{
	int		is_collectible;
	int		has_collision;
	t_vec2	pos;
	t_img	*img;
	double	dist;
	t_vec2	transform;
	int		screen_x;
	int		v_move_screen;
	t_ivec2	size;
	t_ivec2	draw_start;
	t_ivec2	draw_end;
}	t_spr;

typedef struct s_wall_type
{
	char	symbol;
	t_img	*tex_north;
	t_img	*tex_east;
	t_img	*tex_south;
	t_img	*tex_west;
}	t_wall_type;

typedef struct s_row_type
{
	char	symbol;
	t_img	*tex;
}	t_row_type;

typedef struct s_map_pars
{
	char		**lines;
	char		***vars;
	char		**map_wall;
	char		**map_floor;
	char		**map_ceil;
	t_wall_type	*tex_types_wall;
	t_row_type	*tex_types_floor;
	t_row_type	*tex_types_ceil;
	int			door_line_found;
	int			goal_line_found;
	t_img		*tex_door_closed;
	t_img		*tex_door_open;
	t_img		*tex_goal;
}	t_map_pars;

struct s_map
{
	t_map_pars	pars;
	char		*filepath;
	t_ivec2		size;
	t_vec2		start_pos;
	t_vec2		start_dir;
	t_color		fog_color;
	t_color		floor_color;
	t_color		ceil_color;
	t_img		*skybox;
	t_png		*background;
	int			background_offset;
	t_portal	**portals;
	t_cell		**cells;
	int			sprite_len;
	t_spr		**sprites;
	int			to_collect;
	t_a_track	*music_track;
};

typedef struct s_player
{
	double	radius;
	t_vec2	pos;
	t_vec2	prev_pos;
	t_vec2	dir;
	t_vec2	plane;
	int		collected;
	int		is_in_portal;
}	t_player;

# ifdef GL
typedef struct s_frame
{
	GLuint	id;
	t_color	*buf;
	t_ivec2	size;
}	t_frame;
# else
typedef struct s_frame
{
	t_ximg	*img;
	t_ivec2	size;
	double	thickness;
	int		*xmap;
	int		*ymap;
}	t_frame;
# endif

typedef struct s_res
{
	t_ivec2	monitor_size;
	t_ivec2	window_size_default;
	t_ivec2	window_size;
	t_ivec2	window_position;
	double	ratio;
	t_ivec2	res;
	t_ivec2	viewport_size;
	t_ivec2	viewport_offset;
}	t_res;

struct s_man
{
	# ifdef GL
	GLFWwindow		*window;
	GLuint			shader_program;
	GLint			uniform_loc;
	# else
	t_xvar			*xvar;
	t_frame			swap_buf[2];
	# endif
	char			*title;
	t_frame			frame;
	double			*z_buf;
	double			dt;
	long			dt_ms;
	int				fps;
	t_res			res;
	t_game_state	game_state;
	t_player		player;
	double			move_speed;
	double			rotate_speed;
	t_ivec2			move_action;
	int				rotate_action;
	int				l_click_action;
	int				r_click_action;
	t_key			keys[NBR_KEYS + 1];
	t_ivec2			cursor;
	int				show_debug;
	char			**json_paths;
	t_img			**images;
	t_map			**maps;
	int				curr_map;
	int				echolocation;
	double			dof;
	double			fog_width;
	t_ivec2			minimap_offset;
	t_ivec2			minimap_center;
	int				minimap_radius;
	int				minimap_zoom;
	int				minimap_cell_amount;
	t_list			*rays;
	t_audio			audio;
	int				bonus;
};

extern t_man	g_man;

#endif
