#ifndef MLX_INT_H
# define MLX_INT_H

/* Internal settings for the MinilibX */

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

typedef struct s_event_list
{
	int		mask;
	int		(*hook)();
	void	*param;
}	t_event_list;

typedef struct s_win_list
{
	Window				window;
	GC					gc;
	struct s_win_list	*next;
	t_event_list		hooks[MLX_MAX_EVENT];
}	t_win_list;

typedef struct s_img
{
	XImage			*image;
	Pixmap			pix;
	GC				gc;
	int				size_line;
	int				bpp;
	int				width;
	int				height;
	int				type;
	int				format;
	char			*data;
	XShmSegmentInfo	shm;
}	t_img;

typedef struct s_xvar
{
	Display		*display;
	Window		root;
	int			screen;
	int			depth;
	Visual		*visual;
	Colormap	cmap;
	int			private_cmap;
	t_win_list	*win_list;
	int			(*loop_hook)();
	void		*loop_param;
	int			use_xshm;
	int			pshm_format;
	int			do_flush;
	int			decrgb[6];
	Atom		wm_delete_window;
	Atom		wm_protocols;
	int			end_loop;
}	t_xvar;

#endif
