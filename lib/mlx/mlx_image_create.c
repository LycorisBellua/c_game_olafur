#include "mlx.h"

/* To handle X errors */
#define X_SHM_ATTACH 1

int	g_mlx_x_error;

static t_ximg	*mlx_int_new_xshm_image(t_xvar *xvar, int width, int height,
					int format);
static int		shm_att_pb(Display *d, XErrorEvent *ev);
static t_ximg	*mlx_int_new_image(t_xvar *xvar, int width, int height,
					int format);

t_ximg	*mlx_image_create(t_xvar *xvar, int width, int height)
{
	t_ximg	*img;

	if (xvar->use_xshm)
	{
		img = mlx_int_new_xshm_image(xvar, width, height, ZPixmap);
		if (img)
			return (img);
	}
	return (mlx_int_new_image(xvar, width, height, ZPixmap));
}

/* Data malloc: width + 32 (bitmap_pad = 32), *4 = *32 / 8bit */
static t_ximg	*mlx_int_new_xshm_image(t_xvar *xvar, int width, int height,
					int format)
{
	t_ximg	*img;
	int		(*save_handler)();

	img = malloc(sizeof(*img));
	if (!img)
		return ((void *)0);
	bzero(img, sizeof(*img));
	img->data = 0;
	img->image = XShmCreateImage(xvar->display, xvar->visual, xvar->depth,
			format, img->data, &(img->shm), width, height);
	if (!img->image)
	{
		free(img);
		return ((void *)0);
	}
	img->width = width;
	img->height = height;
	img->size_line = img->image->bytes_per_line;
	img->bpp = img->image->bits_per_pixel;
	img->format = format;
	img->shm.shmid = shmget(IPC_PRIVATE, (width + 32) * height * 4,
			IPC_CREAT | 0777);
	if (img->shm.shmid == -1)
	{
		XDestroyImage(img->image);
		free(img);
		return ((void *)0);
	}
	img->data = shmat(img->shm.shmid, 0, 0);
	img->shm.shmaddr = img->data;
	img->image->data = img->data;
	if (img->data == (void *)-1)
	{
		shmctl(img->shm.shmid, IPC_RMID, 0);
		XDestroyImage(img->image);
		free(img);
		return ((void *)0);
	}
	img->shm.readOnly = False;
	g_mlx_x_error = 0;
	save_handler = XSetErrorHandler(shm_att_pb);
	if (!XShmAttach(xvar->display, &(img->shm))
		|| 0 & XSync(xvar->display, False) || g_mlx_x_error)
	{
		XSetErrorHandler(save_handler);
		shmdt(img->data);
		shmctl(img->shm.shmid, IPC_RMID, 0);
		XDestroyImage(img->image);
		free(img);
		return ((void *)0);
	}
	XSetErrorHandler(save_handler);
	shmctl(img->shm.shmid, IPC_RMID, 0);
	if (xvar->pshm_format == format)
	{
		img->pix = XShmCreatePixmap(xvar->display, xvar->root, img->shm.shmaddr,
				&(img->shm), width, height, xvar->depth);
		img->type = MLX_TYPE_SHM_PIXMAP;
	}
	else
	{
		img->pix = XCreatePixmap(xvar->display, xvar->root, width, height,
				xvar->depth);
		img->type = MLX_TYPE_SHM;
	}
	if (xvar->do_flush)
		XFlush(xvar->display);
	return (img);
}

static int	shm_att_pb(Display *d, XErrorEvent *ev)
{
	if (ev->request_code == 146 && ev->minor_code == X_SHM_ATTACH)
		(void)!write(2, WARN_SHM_ATTACH, strlen(WARN_SHM_ATTACH));
	g_mlx_x_error = 1;
	return (0);
}

static t_ximg	*mlx_int_new_image(t_xvar *xvar, int width, int height,
					int format)
{
	t_ximg	*img;

	img = malloc(sizeof(*img));
	if (!img)
		return ((void *)0);
	img->data = malloc((width + 32) * height * 4);
	if (!img->data)
	{
		free(img);
		return ((void *)0);
	}
	bzero(img->data, (width + 32) * height * 4);
	img->image = XCreateImage(xvar->display, xvar->visual, xvar->depth, format,
			0, img->data, width, height, 32, 0);
	if (!img->image)
	{
		free(img->data);
		free(img);
		return ((void *)0);
	}
	img->size_line = img->image->bytes_per_line;
	img->bpp = img->image->bits_per_pixel;
	img->width = width;
	img->height = height;
	img->pix = XCreatePixmap(xvar->display, xvar->root, width, height,
			xvar->depth);
	img->format = format;
	img->type = MLX_TYPE_XIMAGE;
	if (xvar->do_flush)
		XFlush(xvar->display);
	return (img);
}
