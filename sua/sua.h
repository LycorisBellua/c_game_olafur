#ifndef SUA_H
# define SUA_H

/*
	SUA uses 2 more libraries to be included at compile-time: -lsua -lXext -lX11

	The window XY origin is top left corner, Y is positive towards the bottom.
	Color is 0x00RRGGBB.

	`sua_hook` requires some macros and defines from X11/X.h.
*/

# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <X11/Xutil.h>
# include <sys/shm.h>
# include <X11/extensions/XShm.h>
# include <X11/XKBlib.h>

# define SUA_TYPE_SHM_PIXMAP 3
# define SUA_TYPE_SHM 2
# define SUA_TYPE_XIMAGE 1

# define ENV_DISPLAY "DISPLAY"
# define LOCALHOST "localhost"
# define ERR_NO_TRUECOLOR "SUA Error: No TrueColor Visual available.\n"
# define WARN_SHM_ATTACH "SUA Warning: X server can't attach shared memory.\n"

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
	void	(*hook)();
	void	*param;
	int		mask;
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
	int				detectable_repeat;
	t_event_list	hooks[LASTEvent];
	void			(*loop_hook)();
	void			*loop_param;
	int				use_xshm;
	int				pshm_format;
	int				decrgb[6];
	Atom			wm_delete_window;
	Atom			wm_protocols;
	int				end_loop;
}	t_xvar;

t_xvar	*sua_init(void);

void	sua_display_destroy(t_xvar *xvar);
void	sua_screen_size(t_xvar *xvar, int *x, int *y);
void	sua_mouse_hide(t_xvar *xvar);
void	sua_mouse_show(t_xvar *xvar);

int		sua_window_create(t_xvar *xvar, int width, int height, char *title);
void	sua_window_destroy(t_xvar *xvar);
void	sua_window_clear(t_xvar *xvar);
void	sua_window_resize(t_xvar *xvar, int x, int y);
void	sua_window_move(t_xvar *xvar, int x, int y);
void	sua_window_decoration(t_xvar *xvar, int toggle);
void	sua_window_fullscreen(t_xvar *xvar, int toggle);

t_ximg	*sua_image_create(t_xvar *xvar, int width, int height);
void	sua_image_destroy(t_xvar *xvar, t_ximg *img);
void	sua_image_to_window(t_xvar *xvar, t_ximg *img, int x, int y);

void	sua_hook(t_xvar *xvar, int x_event, void (*funct)(), void *param);
void	sua_loop_hook(t_xvar *xvar, void(*funct_ptr)(), void *param);
void	sua_loop(t_xvar *xvar);
void	sua_loop_end(t_xvar *xvar);

#endif
