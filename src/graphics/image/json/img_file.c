#include "olafur.h"

void	free_and_reset_png_file_obj(t_png *file)
{
	free(file->path);
	free(file->path_shadow);
	free(file->buf);
	free(file->buf_shadow);
	free_segments(file);
	memset(file, 0, sizeof(t_png));
	return ;
}

void	free_segments(t_png *file)
{
	size_t	i;

	i = 0;
	while (file->seg && i < file->segment_len)
	{
		free(file->seg[i].id);
		free(file->seg[i].cycle);
		++i;
	}
	free(file->seg);
	file->seg = 0;
	file->segment_len = 0;
	return ;
}
