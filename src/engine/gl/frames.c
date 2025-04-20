#include "cimmerian.h"

void		render_mesh(void);

static int	set_frame(t_man *man, t_frame *f);

int	init_frames(t_man *man)
{
	size_t	i;

	man->curr_frame = 0;
	i = 0;
	while (i < NBR_FRAMES)
	{
		if (!set_frame(man, man->frame + i))
			return (0);
		++i;
	}
	glBindTexture(GL_TEXTURE_2D, man->frame[0].id);
	man->z_buf = malloc(man->frame[0].size.x * sizeof(double));
	if (!man->z_buf)
		return (put_error(man, E_FAIL_MEM, 0));
	return (1);
}

/* Keep using glClear for when the window is resized */
void	clear_frame(t_frame *f)
{
	glClear(GL_COLOR_BUFFER_BIT);
	bzero(f->buf, f->real_size.x * f->real_size.y * sizeof(t_color));
	return ;
}

/*
	Keep using glfwSwapBuffers or the viewport remains black. It's the 
	equivalent to mlx_put_image_to_window
*/
void	display_frame(t_man *man, t_frame *f)
{
	(void)man;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, f->real_size.x, f->real_size.y, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, f->buf);
	render_mesh();
	glfwSwapBuffers(man->window);
	return ;
}

void	free_frames(t_man *man)
{
	size_t	i;

	glBindTexture(GL_TEXTURE_2D, 0);
	i = 0;
	while (i < NBR_FRAMES)
	{
		if (man->frame[i].id > 0)
			glDeleteTextures(1, &man->frame[i].id);
		free(man->frame[i].buf);
		man->frame[i].id = 0;
		man->frame[i].buf = 0;
		++i;
	}
	free(man->z_buf);
	man->z_buf = 0;
	return ;
}

static int	set_frame(t_man *man, t_frame *f)
{
	long	buf_length;

	f->real_size.x = man->res.monitor_size.x;
	f->real_size.y = man->res.monitor_size.y;
	f->thickness = f->real_size.x / man->res.window_size_default.x;
	f->size.x = f->real_size.x / f->thickness;
	f->size.y = f->real_size.y / f->thickness;
	buf_length = f->real_size.x * f->real_size.y * sizeof(t_color);
	f->buf = malloc(buf_length);
	if (!f->buf)
		return (put_error(man, E_FAIL_MEM, 0));
	glGenTextures(1, &f->id);
	glBindTexture(GL_TEXTURE_2D, f->id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);
	return (1);
}
