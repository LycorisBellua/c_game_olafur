#include "cimmerian.h"

static void	play_as_one_shot(t_a_source *s);
static void	*play_as_loop(void *data);
static void	process_buffers(t_a_source *s);

void	audio_source_play(t_a_source *s)
{
	if (!s)
		return ;
	else if (!s->is_loop)
		play_as_one_shot(s);
	else if (!s->loop_thread)
		pthread_create(&s->loop_thread, 0, play_as_loop, s);
	return ;
}

void	audio_source_end_loop_thread(t_a_source *s)
{
	pthread_mutex_lock(&s->is_looping_mutex);
	s->is_looping = 0;
	pthread_mutex_unlock(&s->is_looping_mutex);
	pthread_join(s->loop_thread, 0);
	s->loop_thread = 0;
	return ;
}

static void	play_as_one_shot(t_a_source *s)
{
	if (!s)
		return ;
	alSourcei(s->source, AL_LOOPING, AL_FALSE);
	alSourcePlay(s->source);
	return ;
}

static void	*play_as_loop(void *data)
{
	t_a_source	*s;
	int			is_looping;

	if (!data)
		return (0);
	s = (t_a_source *)data;
	alSourcePlay(s->source);
	pthread_mutex_lock(&s->is_looping_mutex);
	s->is_looping = 1;
	is_looping = 1;
	pthread_mutex_unlock(&s->is_looping_mutex);
	while (is_looping)
	{
		process_buffers(s);
		pthread_mutex_lock(&s->is_looping_mutex);
		is_looping = s->is_looping;
		pthread_mutex_unlock(&s->is_looping_mutex);
	}
	return (0);
}

static void	process_buffers(t_a_source *s)
{
	ALint		processed;
	ALuint		buffer;
	ALint		state;

	processed = 0;
	alGetSourcei(s->source, AL_BUFFERS_PROCESSED, &processed);
	while (processed-- > 0)
	{
		alSourceUnqueueBuffers(s->source, 1, &buffer);
		alSourceQueueBuffers(s->source, 1, &buffer);
	}
	alGetSourcei(s->source, AL_SOURCE_STATE, &state);
	if (state != AL_PLAYING)
		alSourcePlay(s->source);
	return ;
}
