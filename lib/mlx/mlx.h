#ifndef MLX_H
# define MLX_H

/*
	The MinilibX uses 2 more libraries to be included at compile-time:
	-lmlx -lXext -lX11

	The window XY origin is top left corner, Y is positive towards the bottom.
	Color is 0x00RRGGBB.

	`mlx_hook` requires some macros and defines from X11/X.h.
*/

# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <X11/Xutil.h>
# include <sys/shm.h>
# include <X11/extensions/XShm.h>
# include <X11/XKBlib.h>

# define MLX_TYPE_SHM_PIXMAP 3
# define MLX_TYPE_SHM 2
# define MLX_TYPE_XIMAGE 1

# define MLX_MAX_EVENT LASTEvent

# define ENV_DISPLAY "DISPLAY"
# define LOCALHOST "localhost"
# define ERR_NO_TRUECOLOR "MLX Error: No TrueColor Visual available.\n"
# define WARN_SHM_ATTACH "MLX Warning: X server can't attach shared memory.\n"

#define MWM_HINTS_DECORATIONS (1L << 1)

typedef struct
{
	unsigned long	flags;
	unsigned long	functions;
	unsigned long	decorations;
	long			input_mode;
	unsigned long	status;
}	MotifWmHints;

typedef struct s_ximg
{
	XImage			*image;
	Pixmap			pix;
	int				size_line;
	int				bpp;
	int				width;
	int				height;
	int				type;
	int				format;
	char			*data;
	XShmSegmentInfo	shm;
}	t_ximg;

typedef struct s_event_list
{
	int		mask;
	int		(*hook)();
	void	*param;
}	t_event_list;

typedef struct s_xvar
{
	Display			*display;
	Window			root;
	int				screen;
	int				depth;
	Visual			*visual;
	Colormap		cmap;
	int				private_cmap;
	Window			window;
	GC				gc;
	t_event_list	hooks[MLX_MAX_EVENT];
	int				(*loop_hook)();
	void			*loop_param;
	int				use_xshm;
	int				pshm_format;
	int				do_flush;
	int				decrgb[6];
	Atom			wm_delete_window;
	Atom			wm_protocols;
	int				end_loop;
}	t_xvar;

t_xvar	*mlx_init(void);

void	mlx_display_destroy(t_xvar *xvar);
void	mlx_screen_size(t_xvar *xvar, int *x, int *y);

int		mlx_window_create(t_xvar *xvar, int width, int height, char *title);
void	mlx_window_destroy(t_xvar *xvar);
void	mlx_window_clear(t_xvar *xvar);
void	mlx_window_resize(t_xvar *xvar, int x, int y);
void	mlx_window_move(t_xvar *xvar, int x, int y);
void	mlx_window_decoration(t_xvar *xvar, int toggle);
void	mlx_window_fullscreen(t_xvar *xvar, int toggle);

t_ximg	*mlx_image_create(t_xvar *xvar, int width, int height);
void	mlx_image_destroy(t_xvar *xvar, t_ximg *img);
void	mlx_image_to_window(t_xvar *xvar, t_ximg *img, int x, int y);

void	mlx_hook(t_xvar *xvar, int x_event, int x_mask, int (*funct)(),
			void *param);
void	mlx_loop_hook(t_xvar *xvar, int (*funct_ptr)(), void *param);
void	mlx_loop(t_xvar *xvar);
void	mlx_loop_end(t_xvar *xvar);

void	mlx_do_key_autorepeatoff(t_xvar *xvar);
void	mlx_do_key_autorepeaton(t_xvar *xvar);
void	mlx_do_sync(t_xvar *xvar);

void	mlx_mouse_hide(t_xvar *xvar);
void	mlx_mouse_show(t_xvar *xvar);

#endif
