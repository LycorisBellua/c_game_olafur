#include "cimmerian.h"

static t_frame	*create_frame(t_man *man);
static void		free_frame(t_frame **f);

int	create_frames(t_man *man)
{
	size_t	i;
	int		success;

	success = 1;
	man->curr_frame = 0;
	i = 0;
	while (i < NBR_FRAMES)
	{
		man->frame[i] = create_frame(man);
		if (!man->frame[i])
			success = 0;
		++i;
	}
	man->frame[i] = 0;
	if (!success)
		free_frames(man);
	return (success);
}

void	use_frame(t_frame *f)
{
	glBindTexture(GL_TEXTURE_2D, f ? f->id : 0);
	return ;
}

void	clear_drawing(t_frame *f)
{
	size_t	i;
	size_t	len;

	len = f->real_size.x * f->real_size.y;
	bzero(f->buf, len * sizeof(t_color));
	i = 0;
	while (i < len)
	{
		f->buf->a = 255;
		++i;
	}
	return ;
}

void	save_drawing(t_frame *f)
{
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, f->real_size.x, f->real_size.y, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, f->buf);
	return ;
}

void	free_frames(t_man *man)
{
	size_t	i;

	use_frame(0);
	i = 0;
	while (i < NBR_FRAMES)
	{
		free_frame(man->frame + i);
		++i;
	}
	return ;
}

static t_frame	*create_frame(t_man *man)
{
	t_frame	*f;
	long	buf_length;

	f = malloc(sizeof(t_frame));
	if (!f)
	{
		dprintf(STDERR_FILENO,
			"Error: Couldn't allocate memory for frame buffer");
		return (0);
	}

	f->id = -1;
	f->real_size.x = man->res.monitor_size.x;
	f->real_size.y = man->res.monitor_size.y;
	f->thickness = f->real_size.x / man->res.window_size_default.x;
	f->size.x = f->real_size.x / f->thickness;
	f->size.y = f->real_size.y / f->thickness;

	buf_length = f->real_size.x * f->real_size.y * sizeof(t_color);
	f->buf = malloc(buf_length);
	if (!f->buf)
	{
		dprintf(STDERR_FILENO, "Error: Couldn't allocate enough memory for a "
			"frame buffer (= %ld bytes for a %dx%d resolution)\n",
			buf_length, f->real_size.x, f->real_size.y);
		free(f);
		return (0);
	}

	glGenTextures(1, &f->id);
	glBindTexture(GL_TEXTURE_2D, f->id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, 0);
	return (f);
}

static void	free_frame(t_frame **f)
{
	if (!f)
		return ;
	glDeleteTextures(1, &(*f)->id);
	free((*f)->buf);
	free(*f);
	*f = 0;
	return ;
}
