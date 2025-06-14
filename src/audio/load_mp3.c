#include "cimmerian.h"

static drmp3_uint64	read_track(const char *filename, t_a_track *t, drmp3 mp3);
static int			downmix_stereo_to_mono(t_a_track *t,
						drmp3_uint64 samples_read);

int	load_mp3(const char *filename, t_a_track *t, int force_mono)
{
	drmp3			mp3;
	drmp3_uint64	samples_read;

	if (!drmp3_init_file(&mp3, filename, 0))
		return (put_error(0, E_FAIL_MP3_OPEN, filename, 0));
	samples_read = read_track(filename, t, mp3);
	if (!samples_read)
		return (0);
	if (force_mono && mp3.channels == 2)
		return (downmix_stereo_to_mono(t, samples_read));
	t->data_size = samples_read * mp3.channels * sizeof(drmp3_int16);
	if (mp3.channels == 1)
		t->format = AL_FORMAT_MONO16;
	else
		t->format = AL_FORMAT_STEREO16;
	return (1);
}

static drmp3_uint64	read_track(const char *filename, t_a_track *t, drmp3 mp3)
{
	drmp3_uint64	total_pcm_frames;
	drmp3_uint64	samples_read;

	total_pcm_frames = drmp3_get_pcm_frame_count(&mp3);
	t->seconds = (double)total_pcm_frames / mp3.sampleRate;
	t->freq = mp3.sampleRate;
	t->pcm_data = malloc(total_pcm_frames * mp3.channels * sizeof(drmp3_int16));
	if (!t->pcm_data)
	{
		drmp3_uninit(&mp3);
		put_error(0, E_FAIL_MEM, 0, 0);
		return (0);
	}
	samples_read = drmp3_read_pcm_frames_s16(&mp3, total_pcm_frames,
			t->pcm_data);
	drmp3_uninit(&mp3);
	if (!samples_read)
	{
		free(t->pcm_data);
		t->pcm_data = 0;
		put_error(0, E_FAIL_MP3_READ, filename, 0);
	}
	return (samples_read);
}

static int	downmix_stereo_to_mono(t_a_track *t, drmp3_uint64 samples_read)
{
	drmp3_int16		*mono_pcm;
	drmp3_uint64	i;
	int16_t			left;
	int16_t			right;

	mono_pcm = malloc(samples_read * sizeof(drmp3_int16));
	if (!mono_pcm)
	{
		free(t->pcm_data);
		t->pcm_data = 0;
		return (put_error(0, E_FAIL_MEM, 0, 0));
	}
	i = 0;
	while (i < samples_read)
	{
		left = ((drmp3_int16 *)t->pcm_data)[2 * i];
		right = ((drmp3_int16 *)t->pcm_data)[2 * i + 1];
		mono_pcm[i++] = (left + right) / 2;
	}
	free(t->pcm_data);
	t->pcm_data = mono_pcm;
	t->data_size = samples_read * sizeof(drmp3_int16);
	t->format = AL_FORMAT_MONO16;
	return (1);
}
