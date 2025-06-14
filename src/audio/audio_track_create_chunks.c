#include "cimmerian.h"

static int	get_bytes_per_second(t_a_track *t);

void	audio_track_create_chunks(t_a_track *t)
{
	int	bytes_per_chunk;
	int	i;
	int	offset;
	int	size;

	bytes_per_chunk = get_bytes_per_second(t) * 10;
	t->nbr_buffers = t->data_size / bytes_per_chunk
		+ (t->data_size % bytes_per_chunk != 0);
	t->buffers = malloc(t->nbr_buffers * sizeof(ALuint));
	alGenBuffers(t->nbr_buffers, t->buffers);
	i = -1;
	while (++i < t->nbr_buffers)
	{
		offset = i * bytes_per_chunk;
		if (offset + bytes_per_chunk > t->data_size)
			size = t->data_size - offset;
		else
			size = bytes_per_chunk;
		alBufferData(t->buffers[i], t->format, (ALbyte *)t->pcm_data + offset,
			size, t->freq);
	}
	free(t->pcm_data);
	t->pcm_data = 0;
	return ;
}

static int	get_bytes_per_second(t_a_track *t)
{
	if (t->format == AL_FORMAT_MONO16)
		return (t->freq * 2);
	return (t->freq * 4);
}
