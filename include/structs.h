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

typedef struct s_portal
{
	int		is_corridor;
	char	src_cardinal;
	t_ivec2	src_pos;
	char	*path_map;
	int		override_start_pos;
	char	dst_cardinal;
	t_ivec2	dst_pos;
	t_img	*tex;
	int		trigger_opposite;
}	t_portal;

typedef struct s_cell
{
	int			is_empty_space;
	int			is_visible;
	int			is_obstacle;
	int			is_door;
	int			is_goal;
	double		height;
	t_img		*tex_floor;
	t_img		*tex_ceiling;
	t_img		*tex_north;
	t_img		*tex_east;
	t_img		*tex_south;
	t_img		*tex_west;
	t_portal	*portal;
}	t_cell;

typedef struct s_spr
{
	int		is_collectible;
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

struct s_map
{
	char		*filepath;
	char		*map_walls;
	char		*map_ceil_floor;
	t_ivec2		size;
	t_vec2		start_pos;
	t_vec2		start_dir;
	t_color		fog_color;
	t_color		ceiling_color;
	t_color		floor_color;
	t_img		*skybox;
	t_png		*background;
	int			background_offset;
	int			portal_len;
	t_portal	*portals;
	t_cell		**cells;
	int			sprite_len;
	t_spr		**sprites;
};

typedef struct s_player
{
	double	radius;
	t_vec2	pos;
	t_vec2	prev_pos;
	t_vec2	dir;
	t_vec2	plane;
	int		collected;
	int		to_collect;
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
};

extern t_man	g_man;

#endif
