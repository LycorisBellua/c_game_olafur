#include "cimmerian.h"

void		render_mesh(void);

static int	set_frame(t_man *man);

int	init_frames(t_man *man)
{
	if (!set_frame(man))
		return (0);
	glBindTexture(GL_TEXTURE_2D, man->frame.id);
	man->z_buf = malloc(man->frame.size.x * sizeof(double));
	if (!man->z_buf)
		return (put_error(man, E_FAIL_MEM, 0));
	return (1);
}

void	display_frame(t_man *man)
{
	glfwPollEvents();
	glClear(GL_COLOR_BUFFER_BIT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, man->frame.size.x,
		man->frame.size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, man->frame.buf);
	render_mesh();
	glfwSwapBuffers(man->window);
	return ;
}

void	free_frame(t_man *man)
{
	glBindTexture(GL_TEXTURE_2D, 0);
	if (man->frame.id > 0)
		glDeleteTextures(1, &man->frame.id);
	free(man->frame.buf);
	man->frame.id = 0;
	man->frame.buf = 0;
	free(man->z_buf);
	man->z_buf = 0;
	return ;
}

static int	set_frame(t_man *man)
{
	set_ivec2(&man->frame.size, man->res.res.x, man->res.res.y);
	man->frame.buf = malloc(man->frame.size.x * man->frame.size.y
			* sizeof(t_color));
	if (!man->frame.buf)
		return (put_error(man, E_FAIL_MEM, 0));
	glGenTextures(1, &man->frame.id);
	glBindTexture(GL_TEXTURE_2D, man->frame.id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);
	return (1);
}
