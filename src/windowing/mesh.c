#include "olafur.h"

static void		set_vertex_attributes(void);

/* Vertex Buffer Object, Vertex Array Object and Element Buffer Object. */
static GLuint	g_vbo;
static GLuint	g_vao;
static GLuint	g_ebo;

/* For each vertex: its position, then its texture coordinate. */
static GLfloat	g_vertex_data[] = {
	-1.0f, -1.0f, 0.0f, 1.0f,
	1.0f, -1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 0.0f,
	-1.0f, 1.0f, 0.0f, 0.0f,
};

static GLuint	g_indices[] = {
	0, 1, 2,
	0, 2, 3
};

int	create_mesh(void)
{
	glGenBuffers(1, &g_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, g_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_data), g_vertex_data,
		GL_STATIC_DRAW);
	glGenVertexArrays(1, &g_vao);
	glBindVertexArray(g_vao);
	glBindBuffer(GL_ARRAY_BUFFER, g_vbo);
	glGenBuffers(1, &g_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_indices), g_indices,
		GL_STATIC_DRAW);
	set_vertex_attributes();
	return (1);
}

/* 6 is the length of the `g_indices` array. */
void	render_mesh(void)
{
	glBindVertexArray(g_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ebo);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	return ;
}

/* The EBO is freed, then the VBOs related to the VAO, then the VAO itself. */
void	free_mesh(void)
{
	int		i;
	GLint	len;

	if (g_ebo)
		glDeleteBuffers(1, &g_ebo);
	g_ebo = 0;
	if (!g_vao)
		return ;
	glBindVertexArray(g_vao);
	len = 0;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &len);
	i = 0;
	while (i < len)
	{
		g_vbo = 0;
		glGetVertexAttribIuiv(i, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &g_vbo);
		if (g_vbo)
			glDeleteBuffers(1, &g_vbo);
		++i;
	}
	glDeleteVertexArrays(1, &g_vao);
	g_vao = 0;
	return ;
}

/*
	Set up attributes. For example, the position attribute is represented by 
	layout(location = 0) in the vertex shader, then layout(location = 1) is the 
	texture coordinate attribute.

	- `attr_nbr` is the number of attributes (pos and tex coord).
	- `attr_len` being 2 means vertex_data is organized in vec2, so each 
	attribute is made of 2 values.
	- `attr_nbr * attr_len * sizeof(GLfloat)` is the stride, aka how many bytes 
	a vertex is made of in total.
	- Last arg is, in the stride, a pointer to the attribute.
*/
static void	set_vertex_attributes(void)
{
	int	i;
	int	attr_nbr;
	int	attr_len;

	attr_nbr = 2;
	attr_len = 2;
	i = 0;
	while (i < attr_nbr)
	{
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, attr_len, GL_FLOAT, GL_FALSE,
			attr_nbr * attr_len * sizeof(GLfloat),
			(void *)(attr_len * i * sizeof(GLfloat)));
		++i;
	}
	return ;
}
