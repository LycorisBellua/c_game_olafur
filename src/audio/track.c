#include "cimmerian.h"

static t_a_track	*get_track(t_audio *a, const char *filename);
static int			load_mp3(const char *filename, t_a_track *t);
static int			add_track_slot(t_audio *a);
static void			create_chunks(t_a_track *t);

t_a_track	*audio_track_create(t_audio *a, const char *mp3_filename)
{
	char		*abs_path;
	t_a_track	*t;
	int			index;

	abs_path = get_absolute_path(mp3_filename);
	t = get_track(a, abs_path);
	if (t)
	{
		free(abs_path);
		return (t);
	}
	t = calloc(1, sizeof(t_a_track));
	if (!t || !load_mp3(abs_path, t))
	{
		free(abs_path);
		free(t);
		return (0);
	}
	index = add_track_slot(a);
	if (index < 0)
	{
		free(abs_path);
		free(t->pcm_data);
		audio_track_delete(a, &t);
		return (0);
	}
	t->name = abs_path;
	create_chunks(t);
	a->tracks[index] = t;
	return (t);
}

void	audio_track_delete(t_audio *a, t_a_track **track)
{
	/*
		Comb through all sources to find which ones hold a reference to this 
		track, and unset their track before deleting this track.
	*/
	if (a->music && a->music->track == *track)
		audio_source_unset_track(a->music);
	if ((*track)->buffers)
		alDeleteBuffers((*track)->nbr_buffers, (*track)->buffers);
	free((*track)->buffers);
	free((*track)->name);
	free(*track);
	*track = 0;
	return ;
}

void	audio_track_delete_all(t_audio *a)
{
	int	i;

	if (!a->tracks)
		return ;
	i = 0;
	while (a->tracks[i])
	{
		audio_track_delete(a, &a->tracks[i]);
		++i;
	}
	free(a->tracks);
	a->tracks = 0;
	return ;
}

static t_a_track	*get_track(t_audio *a, const char *filename)
{
	int	i;

	if (!a || !a->tracks)
		return (0);
	i = 0;
	while (a->tracks[i])
	{
		if (!strcmp(a->tracks[i]->name, filename))
			return (a->tracks[i]);
		++i;
	}
	return (0);
}

static int	load_mp3(const char *filename, t_a_track *t)
{
	drmp3			mp3;
	drmp3_uint64	total_pcm_frames;
	drmp3_uint64	total_samples;
	drmp3_uint64	samples_read;

	if (!drmp3_init_file(&mp3, filename, NULL))
		return (put_error(0, "Failed to open MP3", filename, 0));
	total_pcm_frames = drmp3_get_pcm_frame_count(&mp3);
	t->seconds = (double)total_pcm_frames / mp3.sampleRate;
	total_samples = total_pcm_frames * mp3.channels;
	t->pcm_data = malloc(total_samples * sizeof(drmp3_int16));
	samples_read = drmp3_read_pcm_frames_s16(&mp3,
		drmp3_get_pcm_frame_count(&mp3), t->pcm_data);
	t->data_size = samples_read * mp3.channels * sizeof(drmp3_int16);
	t->freq = mp3.sampleRate;
	t->format = (mp3.channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
	drmp3_uninit(&mp3);
	return (1);
}

static int	add_track_slot(t_audio *a)
{
	int			count;
	t_a_track	**new_arr;

	count = count_arr_elems((void **)a->tracks);
	new_arr = calloc(count + 1 + 1, sizeof(t_a_track *));
	if (!new_arr)
		return (-1);
	memcpy(new_arr, a->tracks, count * sizeof(t_a_track *));
	free(a->tracks);
	a->tracks = new_arr;
	return (count);
}

static void	create_chunks(t_a_track *t)
{
	int	bytes_per_second;
	int	bytes_per_chunk;
	int	i;
	int	offset;
	int	size;

	bytes_per_second = t->freq * (t->format == AL_FORMAT_MONO16 ? 1 : 2) * 2;
	bytes_per_chunk = bytes_per_second * 10;
	t->nbr_buffers = t->data_size / bytes_per_chunk
		+ (t->data_size % bytes_per_chunk != 0);
	t->buffers = malloc(t->nbr_buffers * sizeof(ALuint));
	alGenBuffers(t->nbr_buffers, t->buffers);
	i = 0;
	while (i < t->nbr_buffers)
	{
		offset = i * bytes_per_chunk;
		if (offset + bytes_per_chunk > t->data_size)
			size = t->data_size - offset;
		else
			size = bytes_per_chunk;
		alBufferData(t->buffers[i], t->format, (ALbyte *)t->pcm_data + offset,
			size, t->freq);
		++i;
	}
	free(t->pcm_data);
	t->pcm_data = 0;
	return ;
}
