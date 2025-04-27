#include "mlx.h"

void	mlx_image_destroy(t_xvar *xvar, t_ximg *img)
{
	if (img->type == MLX_TYPE_SHM_PIXMAP || img->type == MLX_TYPE_SHM)
	{
		XShmDetach(xvar->display, &(img->shm));
		shmdt(img->shm.shmaddr);
		/* shmctl IPC_RMID already done */
	}
	XDestroyImage(img->image); /* For image & shm-image. Also free img->data */
	XFreePixmap(xvar->display, img->pix);
	free(img);
	return ;
}
