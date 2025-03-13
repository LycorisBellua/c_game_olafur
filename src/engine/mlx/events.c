#include "cimmerian.h"

static int	on_resize(t_win *win);

int	on_close(t_win *win)
{
	release_resources(win);
	exit(0);
	return (0);
}

int	on_mouse_button(int button, int x, int y, t_win *win)
{
	int	thickness;

	if (button == BTN_CLICK_LEFT)
	{
		thickness = win->frame[0].thickness;
		win->click = 1;
		set_ivec2(&win->click_pos, x / thickness, y / thickness);
	}
	else if (button == BTN_SCROLL_UP && win->zoom < 10)
	{
		if (win->zoom < 3)
			win->zoom += 1;
		else
			win->zoom += 2;
	}
	else if (button == BTN_SCROLL_DOWN && win->zoom > 1)
	{
		if (win->zoom <= 3)
			win->zoom -= 1;
		else
			win->zoom -= 2;
	}
	return (0);
}

/*
	else if (keycode >= KEY_A && keycode <= (KEY_A + 25))
	printf("%c (%d)\n", keycode - KEY_A + 'A', keycode);
*/
int	on_key_press(int keycode, t_win *win)
{
	t_ivec2	move;

	if (keycode == KEY_ESCAPE)
		return (on_close(win));
	else if (keycode == KEY_F11)
		return (on_resize(win));
	else
	{
		set_ivec2(&move, 0, 0);
		if (keycode == KEY_W || keycode == KEY_UP)
			move.y = -1;
		else if (keycode == KEY_A || keycode == KEY_LEFT)
			move.x = -1;
		else if (keycode == KEY_S || keycode == KEY_DOWN)
			move.y = 1;
		else if (keycode == KEY_D || keycode == KEY_RIGHT)
			move.x = 1;
		if (!win->click_pos.x && !win->click_pos.y)
			move_player(win, move);
	}
	return (0);
}

static int	on_resize(t_win *win)
{
	t_ivec2	size;
	int		thickness;

	mlx_get_screen_size(win->mlx, &size.x, &size.y);
	thickness = size.x / RES_WIDTH;
	if (thickness > 1)
	{
		--thickness;
		set_ivec2(&size, RES_WIDTH * thickness, RES_HEIGHT * thickness);
	}
	if (win->size.x == size.x)
		set_ivec2(&size, RES_WIDTH, RES_HEIGHT);
	mlx_destroy_window(win->mlx, win->win);
	mlx_destroy_image(win->mlx, win->frame[0].img);
	mlx_destroy_image(win->mlx, win->frame[1].img);
	mlx_destroy_image(win->mlx, win->frame[2].img);
	if (!create_window(win, size.x, size.y))
	{
		release_resources(win);
		exit(1);
	}
	return (0);
}
