#include "mlx.h"

static void set_detectable_repeat(t_xvar *xvar);
static int	mlx_int_get_visual(t_xvar *xvar);
static void	mlx_int_deal_shm(t_xvar *xvar);
static void	mlx_int_rgb_conversion(t_xvar *xvar);

t_xvar	*mlx_init(void)
{
	t_xvar	*xvar;

	if (xvar)
		return (xvar);
	xvar = calloc(1, sizeof(*xvar));
	if (!xvar)
		return ((void *)0);
	xvar->display = XOpenDisplay("");
	if (!xvar->display)
	{
		free(xvar);
		return ((void *)0);
	}
	set_detectable_repeat(xvar);
	xvar->screen = DefaultScreen(xvar->display);
	xvar->root = DefaultRootWindow(xvar->display);
	xvar->cmap = DefaultColormap(xvar->display, xvar->screen);
	xvar->depth = DefaultDepth(xvar->display, xvar->screen);
	if (mlx_int_get_visual(xvar) == -1)
	{
		printf(ERR_NO_TRUECOLOR);
		exit(1);
	}
	xvar->do_flush = 1;
	xvar->wm_delete_window = XInternAtom(xvar->display, "WM_DELETE_WINDOW",
			False);
	xvar->wm_protocols = XInternAtom(xvar->display, "WM_PROTOCOLS", False);
	mlx_int_deal_shm(xvar);
	if (xvar->private_cmap)
		xvar->cmap = XCreateColormap(xvar->display, xvar->root, xvar->visual,
				AllocNone);
	mlx_int_rgb_conversion(xvar);
	return (xvar);
}

/*
	`XkbSetDetectableAutoRepeat` deactivates the default behavior of sending 
	continuous Press and Release key events while a key is being held. Instead, 
	Press events are being sent until a true Release. Not all systems support 
	this feature however, despite being from 2001. A fallback might therefore 
	be necessary, which would decrease performance but still better than 
	nothing.
*/
static void set_detectable_repeat(t_xvar *xvar)
{
	int	supported;

	if (XkbSetDetectableAutoRepeat(xvar->display, 1, &supported) && supported)
		xvar->detectable_repeat = 1;
	return ;
}

/* We need a private colormap for non-default Visual. */
static int	mlx_int_get_visual(t_xvar *xvar)
{
	XVisualInfo	*vi;
	XVisualInfo	template;
	int			nb_item;

	xvar->private_cmap = 0;
	xvar->visual = DefaultVisual(xvar->display, xvar->screen);
	if (xvar->visual->class == TrueColor)
		return (0);
	template.class = TrueColor;
	template.depth = xvar->depth;
	vi = XGetVisualInfo(xvar->display, VisualDepthMask | VisualClassMask,
			&template, &nb_item);
	if (!vi)
		return (-1);
	xvar->visual = vi->visual;
	xvar->private_cmap = 1;
	return (0);
}

/*
	pshm_format of -1: Not XYBitmap|XYPixmap|ZPixmap
	alpha libX need a check of the DISPLAY env var, or shm is allowed in remote 
	Xserver connections.
*/
static void	mlx_int_deal_shm(t_xvar *xvar)
{
	int		use_pshm;
	int		bidon;
	char	*dpy;
	char	buff[33];

	xvar->use_xshm = XShmQueryVersion(xvar->display, &bidon, &bidon,
			&(use_pshm));
	if (xvar->use_xshm && use_pshm)
		xvar->pshm_format = XShmPixmapFormat(xvar->display);
	else
		xvar->pshm_format = -1;
	gethostname(buff, 32);
	dpy = getenv(ENV_DISPLAY);
	if (dpy && strlen(dpy) && *dpy != ':' && strncmp(dpy, buff, strlen(buff))
		&& strncmp(dpy, LOCALHOST, strlen(LOCALHOST)))
	{
		xvar->pshm_format = -1;
		xvar->use_xshm = 0;
	}
	return ;
}

/* TrueColor Visual is needed to have *_mask correctly set */
static void	mlx_int_rgb_conversion(t_xvar *xvar)
{
	bzero(xvar->decrgb, 6 * sizeof(int));
	while (!(xvar->visual->red_mask & 1))
	{
		xvar->visual->red_mask >>= 1;
		xvar->decrgb[0]++;
	}
	while (xvar->visual->red_mask & 1)
	{
		xvar->visual->red_mask >>= 1;
		xvar->decrgb[1]++;
	}
	while (!(xvar->visual->green_mask & 1))
	{
		xvar->visual->green_mask >>= 1;
		xvar->decrgb[2]++;
	}
	while (xvar->visual->green_mask & 1)
	{
		xvar->visual->green_mask >>= 1;
		xvar->decrgb[3]++;
	}
	while (!(xvar->visual->blue_mask & 1))
	{
		xvar->visual->blue_mask >>= 1;
		xvar->decrgb[4]++;
	}
	while (xvar->visual->blue_mask & 1)
	{
		xvar->visual->blue_mask >>= 1;
		xvar->decrgb[5]++;
	}
	return ;
}
