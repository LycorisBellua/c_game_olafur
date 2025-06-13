#include "cimmerian.h"

static void	*play_as_loop(void *data);
static void	play_as_one_shot(t_a_source *s);

t_a_source	*audio_source_create(t_a_track *t, int is_loop)
{
	t_a_source	*s;

	s = calloc(1, sizeof(t_a_source));
	if (!s)
		return (0);
	alGenSources(1, &s->source);
	s->track = t;
	if (t)
		alSourceQueueBuffers(s->source, t->nbr_buffers, t->buffers);
	s->is_loop = is_loop;
	if (s->is_loop)
		pthread_mutex_init(&s->is_looping_mutex, NULL);
	return (s);
}

void	audio_source_change_track(t_a_source *s, t_a_track *t)
{
	if (!s)
		return ;
	audio_source_unset_track(s);
	if (t)
		alSourceQueueBuffers(s->source, t->nbr_buffers, t->buffers);
	s->track = t;
	return ;
}

void	audio_source_unset_track(t_a_source *s)
{
	ALint	queued;
	ALuint	buffer;

	if (!s)
		return ;
	if (s->loop_thread)
	{
		pthread_mutex_lock(&s->is_looping_mutex);
		s->is_looping = 0;
		pthread_mutex_unlock(&s->is_looping_mutex);
		pthread_join(s->loop_thread, NULL);
		s->loop_thread = 0;
	}
	s->track = 0;
	alSourceStop(s->source);
	queued = 0;
	alGetSourcei(s->source, AL_BUFFERS_QUEUED, &queued);
	while (queued-- > 0)
		alSourceUnqueueBuffers(s->source, 1, &buffer);
	return ;
}

void	audio_source_delete(t_a_source **s)
{
	if (!*s)
		return ;
	audio_source_unset_track(*s);
	if ((*s)->is_loop)
		pthread_mutex_destroy(&(*s)->is_looping_mutex);
	alDeleteSources(1, &(*s)->source);
	free(*s);
	*s = 0;
	return ;
}

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

void	audio_source_pause(t_a_source *s)
{
	if (!s)
		return ;
	if (s->loop_thread)
	{
		pthread_mutex_lock(&s->is_looping_mutex);
		s->is_looping = 0;
		pthread_mutex_unlock(&s->is_looping_mutex);
		pthread_join(s->loop_thread, NULL);
		s->loop_thread = 0;
	}
	alSourcePause(s->source);
	return ;
}

void	audio_source_stop(t_a_source *s)
{
	if (!s)
		return ;
	if (s->loop_thread)
	{
		pthread_mutex_lock(&s->is_looping_mutex);
		s->is_looping = 0;
		pthread_mutex_unlock(&s->is_looping_mutex);
		pthread_join(s->loop_thread, NULL);
		s->loop_thread = 0;
	}
	alSourceStop(s->source);
	return ;
}

static void	*play_as_loop(void *data)
{
	t_a_source	*s;
	int			is_looping;
	ALint		processed;
	ALuint		buffer;
	ALint		state;

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
		pthread_mutex_lock(&s->is_looping_mutex);
		is_looping = s->is_looping;
		pthread_mutex_unlock(&s->is_looping_mutex);
	}
	return (0);
}

static void	play_as_one_shot(t_a_source *s)
{
	if (!s)
		return ;
	alSourcei(s->source, AL_LOOPING, AL_FALSE);
	alSourcePlay(s->source);
	return ;
}
