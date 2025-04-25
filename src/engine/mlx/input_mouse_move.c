#include "cimmerian.h"

static void	rotate_on_click(t_man *man);
static void	init_rotate_click(t_man *man, int *x_first_click, int *x_max_diff);
static int	get_diff_perc(t_man *man, int diff);

int	mouse_move_callback(int x, int y, t_man *man)
{
	man->cursor.x = ((int)x - man->res.viewport_offset.x) * man->res.res.x
		/ man->res.viewport_size.x;
	man->cursor.y = ((int)y - man->res.viewport_offset.y) * man->res.res.y
		/ man->res.viewport_size.y;
	if (man->cursor.x < 0 || man->cursor.x >= man->res.res.x
		|| man->cursor.y < 0 || man->cursor.y >= man->res.res.y)
	{
		set_ivec2(&man->cursor, -1, -1);
		mlx_mouse_show(man->xvar);
		return (0);
	}
	mlx_mouse_hide(man->xvar);
	if (man->game_state == GAME_STATE_PLAY)
		rotate_on_click(man);
	return (0);
}

static void	rotate_on_click(t_man *man)
{
	static int	x_first_click;
	static int	x_max_diff;
	int			diff;

	if (!man->r_click_action)
		x_first_click = -1;
	else if (x_first_click < 0)
		init_rotate_click(man, &x_first_click, &x_max_diff);
	if (x_first_click >= 0)
	{
		diff = man->cursor.x - x_first_click;
		if ((diff > 0 && diff > x_max_diff) || (diff < 0 && diff < x_max_diff))
			x_max_diff = diff;
		if ((diff > 0 && diff < x_max_diff) || (diff < 0 && diff > x_max_diff))
		{
			x_first_click += x_max_diff;
			diff = man->cursor.x - x_first_click;
			x_max_diff = diff;
		}
		rotate_player(man,
			RAD_1 * 2 * get_diff_perc(man, diff) * norm(diff) * man->dt);
	}
	return ;
}

static void	init_rotate_click(t_man *man, int *x_first_click, int *x_max_diff)
{
	*x_first_click = man->cursor.x;
	*x_max_diff = 0;
	return ;
}

static int	get_diff_perc(t_man *man, int diff)
{
	return (max(10, abs(diff) * 100 / man->res.window_size_default.x));
}
