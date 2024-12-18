#include "cimmerian.h"

void	set_dt_and_fps(void)
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
	g_man.dt = delta_s + delta_ms;
	if (!g_man.dt)
		g_man.fps = 0;
	else
		g_man.fps = (int)(1 / g_man.dt);
	prev_time = curr_time;
	return ;
}
