#include "olafur.h"

static double	compute_dt(struct timeval prev_time, struct timeval curr_time);

void	set_dt_and_fps(t_man *man)
{
	static struct timeval	prev_time;
	struct timeval			curr_time;

	gettimeofday(&curr_time, 0);
	if (!prev_time.tv_sec && !prev_time.tv_usec)
		prev_time = curr_time;
	man->dt = compute_dt(prev_time, curr_time);
	man->dt_ms = man->dt * 1000;
	if (!man->dt)
		man->fps = 0;
	else
		man->fps = (int)(1 / man->dt);
	prev_time = curr_time;
	return ;
}

void	display_fps(t_man *man)
{
	static long	dt_ms;
	static int	smoothed_fps;
	char		*s;
	char		*num;
	t_ivec2		pos;

	dt_ms += man->dt_ms;
	if (dt_ms >= 250)
	{
		smoothed_fps = man->fps;
		dt_ms -= 250;
	}
	num = itoa(smoothed_fps);
	s = strjoin("FPS: ", num);
	set_ivec2(&pos, 20, 40);
	draw_font_default(man, &pos, s);
	free(s);
	free(num);
	return ;
}

static double	compute_dt(struct timeval prev_time, struct timeval curr_time)
{
	double	delta_s;
	double	delta_ms;

	delta_s = (double)(curr_time.tv_sec - prev_time.tv_sec);
	delta_ms = (double)(curr_time.tv_usec - prev_time.tv_usec) / 1000000.0;
	if (delta_ms < 0)
	{
		delta_s -= 1.0;
		delta_ms += 1.0;
	}
	return (delta_s + delta_ms);
}
