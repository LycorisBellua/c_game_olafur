#include "cimmerian.h"

void				audio_track_create_chunks(t_a_track *t);

static t_a_track	*get_track(t_audio *a, char *abs_path);
static t_a_track	*allocate_track(char *abs_path, int force_mono);
static int			add_track_slot(t_audio *a);

t_a_track	*audio_track_create(t_audio *a, const char *mp3_filename,
	int force_mono)
{
	char		*abs_path;
	t_a_track	*t;
	int			index;

	abs_path = get_absolute_path(mp3_filename);
	if (!abs_path)
		return (0);
	t = get_track(a, abs_path);
	if (t)
		return (t);
	t = allocate_track(abs_path, force_mono);
	if (!t)
		return (0);
	index = add_track_slot(a);
	if (index < 0)
	{
		free(abs_path);
		free(t->pcm_data);
		audio_track_delete(a, &t);
		return (0);
	}
	t->name = abs_path;
	audio_track_create_chunks(t);
	a->tracks[index] = t;
	return (t);
}

static t_a_track	*get_track(t_audio *a, char *abs_path)
{
	int	i;

	if (!a || !a->tracks || !abs_path)
		return (0);
	i = 0;
	while (a->tracks[i])
	{
		if (!strcmp(a->tracks[i]->name, abs_path))
		{
			free(abs_path);
			return (a->tracks[i]);
		}
		++i;
	}
	return (0);
}

static t_a_track	*allocate_track(char *abs_path, int force_mono)
{
	t_a_track	*t;

	t = calloc(1, sizeof(t_a_track));
	if (!t || !load_mp3(abs_path, t, force_mono))
	{
		free(abs_path);
		free(t);
		return (0);
	}
	return (t);
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
