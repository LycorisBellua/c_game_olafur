#include "cimmerian.h"

static void	play_as_loop(t_a_source *s);
static void	play_as_one_shot(t_a_source *s);

t_a_source	*audio_source_create(t_a_track *t)
{
	t_a_source	*s;

	s = calloc(1, sizeof(t_a_source));
	if (!s)
		return (0);
	alGenSources(1, &s->source);
	s->track = t;
	if (t)
		alSourceQueueBuffers(s->source, t->nbr_buffers, t->buffers);
	s->is_loop = 0;
	s->is_running = 0;
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
	s->is_running = 0;
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
	alDeleteSources(1, &(*s)->source);
	free(*s);
	*s = 0;
	return ;
}

void	audio_source_play(t_a_source *s)
{
	if (!s)
		return ;
	else if (s->is_loop)
		play_as_loop(s);
	else
		play_as_one_shot(s);
	return ;
}

void	audio_source_pause(t_a_source *s)
{
	if (!s)
		return ;
	s->is_running = 0;
	alSourcePause(s->source);
	return ;
}

void	audio_source_stop(t_a_source *s)
{
	if (!s)
		return ;
	s->is_running = 0;
	alSourceStop(s->source);
	return ;
}

static void	play_as_loop(t_a_source *s)
{
	// TODO: Run loop in another thread
	ALint	processed;
	ALuint	buffer;
	ALint	state;

	if (!s)
		return ;
	s->is_running = 1;
	alSourcePlay(s->source);
	while (s->is_running)
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
	}
	return ;
}

static void	play_as_one_shot(t_a_source *s)
{
	if (!s)
		return ;
	s->is_running = 1;
	alSourcei(s->source, AL_LOOPING, AL_FALSE);
	alSourcePlay(s->source);
	return ;
}
