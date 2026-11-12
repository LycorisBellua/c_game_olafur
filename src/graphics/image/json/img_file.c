#include "olafur.h"

/*
	`i` is owned by the caller, so that an aborted parse can't leave a stale 
	index behind for the next JSON file.
*/
int	set_png_file_obj(t_png *file, char **lines, size_t *i)
{
	while (lines[*i])
	{
		if (is_field(lines[*i], "path"))
			set_string_value(&file->path, lines[*i]);
		else if (is_field(lines[*i], "path_shadow"))
			set_string_value(&file->path_shadow, lines[*i]);
		else if (is_field(lines[*i], "segments"))
			parse_segments(file, lines, i);
		else if (lines[*i][0] == '}')
		{
			++*i;
			break ;
		}
		++*i;
	}
	return (!!lines[*i]);
}

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

/* Also used when "segments" is given twice, so that the first ones don't leak. */
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

/* Replaces the previous value, so that a field given twice doesn't leak. */
void	set_string_value(char **dst, char *line)
{
	free(*dst);
	*dst = get_string_value(line);
	return ;
}
