#include "olafur.h"

void		render_mesh(void);

static int	set_frame(t_man *man);

int	init_frame(t_man *man)
{
	if (!set_frame(man))
		return (0);
	glBindTexture(GL_TEXTURE_2D, man->frame.id);
	man->z_buf = malloc(man->frame.size.x * sizeof(float));
	if (!man->z_buf)
		return (put_error(man, E_FAIL_MEM, 0, 0));
	return (1);
}

void	display_frame(t_man *man)
{
	int		upload_pbo;
	int		map_pbo;
	void	*ptr;
	int		buf_size;

	buf_size = man->frame.size.x * man->frame.size.y * sizeof(t_color);
	upload_pbo = man->pbo_index;
	map_pbo = 1 - man->pbo_index;
	glfwPollEvents();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, man->pbo[upload_pbo]);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, man->frame.size.x,
		man->frame.size.y, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	render_mesh();
	glfwSwapBuffers(man->window);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, man->pbo[map_pbo]);
	ptr = glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
	if (ptr)
	{
		memcpy(ptr, man->frame.buf, buf_size);
		glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
	}
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
	man->pbo_index = map_pbo;
	return ;
}

void	free_frame(t_man *man)
{
	glBindTexture(GL_TEXTURE_2D, 0);
	if (man->frame.id > 0)
		glDeleteTextures(1, &man->frame.id);
   glDeleteBuffers(2, man->pbo);
	free(man->frame.buf);
	man->frame.id = 0;
	man->frame.buf = 0;
	free(man->z_buf);
	man->z_buf = 0;
	return ;
}

static int	set_frame(t_man *man)
{
   int buf_size;

	set_ivec2(&man->frame.size, man->res.res.x, man->res.res.y);
   buf_size = man->frame.size.x * man->frame.size.y * sizeof(t_color);
	man->frame.buf = malloc(buf_size);
	if (!man->frame.buf)
		return (put_error(man, E_FAIL_MEM, 0, 0));
	glGenTextures(1, &man->frame.id);
	glBindTexture(GL_TEXTURE_2D, man->frame.id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, man->frame.size.x,
       man->frame.size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, man->frame.buf);
   glGenBuffers(2, man->pbo);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, man->pbo[0]);
	glBufferData(GL_PIXEL_UNPACK_BUFFER, buf_size, 0, GL_STREAM_DRAW);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, man->pbo[1]);
	glBufferData(GL_PIXEL_UNPACK_BUFFER, buf_size, 0, GL_STREAM_DRAW);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
	man->pbo_index = 0;
	return (1);
}
