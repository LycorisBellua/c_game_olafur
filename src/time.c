#include "cimmerian.h"

void	set_dt_and_fps(t_man *man)
{
	static struct timeval	prev_time;
	struct timeval			curr_time;
	double					delta_s;
	double					delta_ms;

	gettimeofday(&curr_time, NULL);
	if (!prev_time.tv_sec && !prev_time.tv_usec)
		prev_time = curr_time;
	delta_s = (double)(curr_time.tv_sec - prev_time.tv_sec);
	delta_ms = (double)(curr_time.tv_usec - prev_time.tv_usec) / 1000000.0;
	if (delta_ms < 0)
	{
		delta_s -= 1.0;
		delta_ms += 1.0;
	}
	man->dt = delta_s + delta_ms;
	man->dt_ms = man->dt * 1000;
	if (!man->dt)
		man->fps = 0;
	else
		man->fps = (int)(1 / man->dt);
	prev_time = curr_time;
	return ;
}

void	display_fps(t_man *man, t_frame *f, t_ivec2 pos)
{
	static long	dt_ms;
	static int	smoothed_fps;
	char		*s;
	char		*num;

	dt_ms += man->dt_ms;
	if (dt_ms >= 250)
	{
		smoothed_fps = man->fps;
		dt_ms -= 250;
	}
	num = itoa_dec(smoothed_fps);
	s = strjoin("FPS: ", num);
	draw_font_default(man, f, &pos, s);
	free(s);
	free(num);
	return ;
}
