#ifndef MLX_H
# define MLX_H

/*
	The MinilibX uses 2 more libraries to be included at compile-time:
	-lmlx -lXext -lX11

	The window XY origin is top left corner, Y is positive towards the bottom.
	Color is 0x00RRGGBB.

	`mlx_hook` requires some macros and defines from X11/X.h.
*/

void	*mlx_init(void);

void	*mlx_new_window(void *mlx_ptr, int size_x, int size_y, char *title);
int		mlx_get_screen_size(void *mlx_ptr, int *sizex, int *sizey);
int		mlx_resize_window(void *mlx_ptr, void *win_ptr, int width, int height);
int		mlx_clear_window(void *mlx_ptr, void *win_ptr);

void	*mlx_new_image(void *mlx_ptr, int width, int height);
char	*mlx_get_data_addr(void *img_ptr, int *bits_per_pixel, int *size_line,
			int *endian);
int		mlx_put_image_to_window(void *mlx_ptr, void *win_ptr, void *img_ptr,
			int x, int y);

int		mlx_hook(void *win_ptr, int x_event, int x_mask, int (*funct)(),
			void *param);
int		mlx_loop_hook(void *mlx_ptr, int (*funct_ptr)(), void *param);
int		mlx_loop(void *mlx_ptr);
int		mlx_loop_end(void *mlx_ptr);

int		mlx_do_key_autorepeatoff(void *mlx_ptr);
int		mlx_do_key_autorepeaton(void *mlx_ptr);
int		mlx_do_sync(void *mlx_ptr);

int		mlx_mouse_get_pos(void *mlx_ptr, void *win_ptr, int *x, int *y);
int		mlx_mouse_move(void *mlx_ptr, void *win_ptr, int x, int y);
int		mlx_mouse_hide(void *mlx_ptr, void *win_ptr);
int		mlx_mouse_show(void *mlx_ptr, void *win_ptr);

int		mlx_destroy_window(void *mlx_ptr, void *win_ptr);
int		mlx_destroy_image(void *mlx_ptr, void *img_ptr);
int		mlx_destroy_display(void *mlx_ptr);

#endif
