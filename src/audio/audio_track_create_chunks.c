#include "olafur.h"

static int	get_bytes_per_second(t_a_track *t);
static void	fill_buffers(t_a_track *t, int bytes_per_chunk);

int	audio_track_create_chunks(t_a_track *t)
{
	int	bytes_per_chunk;

	bytes_per_chunk = get_bytes_per_second(t) * 10;
	t->nbr_buffers = t->data_size / bytes_per_chunk
		+ (t->data_size % bytes_per_chunk != 0);
	t->buffers = malloc(t->nbr_buffers * sizeof(ALuint));
	if (!t->buffers)
	{
		free(t->pcm_data);
		t->pcm_data = 0;
		return (put_error(E_FAIL_MEM, 0, 0));
	}
	alGenBuffers(t->nbr_buffers, t->buffers);
	fill_buffers(t, bytes_per_chunk);
	free(t->pcm_data);
	t->pcm_data = 0;
	return (1);
}

static int	get_bytes_per_second(t_a_track *t)
{
	if (t->format == AL_FORMAT_MONO16)
		return (t->freq * 2);
	return (t->freq * 4);
}

static void	fill_buffers(t_a_track *t, int bytes_per_chunk)
{
	int	i;
	int	offset;
	int	size;

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
	return ;
}
