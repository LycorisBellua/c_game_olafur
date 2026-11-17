#include "olafur.h"

static void	copy_frame_to_pbo(t_man *man, GLuint pbo);
static int	set_frame(t_man *man);

int	init_frame(t_man *man)
{
	if (!set_frame(man))
		return (0);
	glBindTexture(GL_TEXTURE_2D, man->frame.id);
	man->z_buf = malloc(man->frame.size.x * sizeof(float));
	if (!man->z_buf)
		return (put_error(E_FAIL_MEM, 0, 0));
	man->ray_len = malloc(man->frame.size.x * sizeof(float));
	if (!man->ray_len)
		return (put_error(E_FAIL_MEM, 0, 0));
	man->fc_depth = malloc(man->frame.size.x * man->frame.size.y
		* sizeof(float));
	if (!man->fc_depth)
		return (put_error(E_FAIL_MEM, 0, 0));
	return (1);
}

/*
	Two PBOs (pixel buffer objects) take turns: while the texture is updated 
	from one, the next frame is copied into the other, so the CPU and the GPU 
	don't wait on each other. The frame shown is the one drawn the loop before.
*/
void	display_frame(t_man *man)
{
	int	upload_pbo;
	int	map_pbo;

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
	copy_frame_to_pbo(man, man->pbo[map_pbo]);
	man->pbo_index = map_pbo;
	return ;
}

void	free_frame(t_man *man)
{
	if (man->gl_loaded)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		if (man->frame.id > 0)
			glDeleteTextures(1, &man->frame.id);
		glDeleteBuffers(2, man->pbo);
	}
	free(man->frame.buf);
	man->frame.id = 0;
	man->frame.buf = 0;
	free(man->z_buf);
	man->z_buf = 0;
	free(man->ray_len);
	man->ray_len = 0;
	free(man->fc_depth);
	man->fc_depth = 0;
	return ;
}

static int	set_frame(t_man *man)
{
	int	size;

	set_ivec2(&man->frame.size, man->res.res.x, man->res.res.y);
	size = man->frame.size.x * man->frame.size.y * sizeof(t_color);
	man->frame.buf = calloc(man->frame.size.x * man->frame.size.y,
		sizeof(t_color));
	if (!man->frame.buf)
		return (put_error(E_FAIL_MEM, 0, 0));
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
	glBufferData(GL_PIXEL_UNPACK_BUFFER, size, man->frame.buf, GL_STREAM_DRAW);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, man->pbo[1]);
	glBufferData(GL_PIXEL_UNPACK_BUFFER, size, man->frame.buf, GL_STREAM_DRAW);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
	man->pbo_index = 0;
	return (1);
}

static void	copy_frame_to_pbo(t_man *man, GLuint pbo)
{
	void	*ptr;

	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo);
	ptr = glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
	if (ptr)
	{
		memcpy(ptr, man->frame.buf,
			man->frame.size.x * man->frame.size.y * sizeof(t_color));
		glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
	}
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
	return ;
}
