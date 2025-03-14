#include "cimmerian.h"

static int	on_resize(t_man *man);

int	on_close(t_man *man)
{
	release_resources(man);
	exit(0);
	return (0);
}

int	on_mouse_button(int button, int x, int y, t_man *man)
{
	int	thickness;

	if (button == BTN_CLICK_LEFT)
	{
		thickness = man->frame[0].thickness;
		man->click = 1;
		set_ivec2(&man->click_pos, x / thickness, y / thickness);
	}
	else if (button == BTN_SCROLL_UP && man->zoom < 10)
	{
		if (man->zoom < 3)
			man->zoom += 1;
		else
			man->zoom += 2;
	}
	else if (button == BTN_SCROLL_DOWN && man->zoom > 1)
	{
		if (man->zoom <= 3)
			man->zoom -= 1;
		else
			man->zoom -= 2;
	}
	return (0);
}

/*
	else if (keycode >= KEY_A && keycode <= (KEY_A + 25))
	printf("%c (%d)\n", keycode - KEY_A + 'A', keycode);
*/
int	on_key_press(int keycode, t_man *man)
{
	t_ivec2	move;

	if (keycode == KEY_ESCAPE)
		return (on_close(man));
	else if (keycode == KEY_F11)
		return (on_resize(man));
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
		if (!man->click_pos.x && !man->click_pos.y)
			move_player(man, move);
	}
	return (0);
}

static int	on_resize(t_man *man)
{
	t_ivec2	size;
	int		thickness;

	mlx_get_screen_size(man->mlx, &size.x, &size.y);
	thickness = size.x / RES_WIDTH;
	if (thickness > 1)
	{
		--thickness;
		set_ivec2(&size, RES_WIDTH * thickness, RES_HEIGHT * thickness);
	}
	if (man->size.x == size.x)
		set_ivec2(&size, RES_WIDTH, RES_HEIGHT);
	mlx_destroy_mandow(man->mlx, man->window);
	mlx_destroy_image(man->mlx, man->frame[0].img);
	mlx_destroy_image(man->mlx, man->frame[1].img);
	mlx_destroy_image(man->mlx, man->frame[2].img);
	if (!create_mandow(man, size.x, size.y))
	{
		release_resources(man);
		exit(1);
	}
	return (0);
}
