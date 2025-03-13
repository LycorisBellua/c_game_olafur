# define BTN_CLICK_LEFT  1
# define BTN_CLICK_WHEEL 2
# define BTN_CLICK_RIGHT 3
# define BTN_SCROLL_UP   4
# define BTN_SCROLL_DOWN 5
# define KEY_W 119
# define KEY_A 97
# define KEY_S 115
# define KEY_D 100
# define KEY_ENTER  65293
# define KEY_ESCAPE 65307
# define KEY_LEFT   65361
# define KEY_UP     65362
# define KEY_RIGHT  65363
# define KEY_DOWN   65364
# define KEY_F11    65480

typedef struct s_frame
{
	void	*img;
	char	*addr;
	int		bpp;
	int		line_length;
	int		endian;
	int		thickness;
	t_ivec2	size;
}	t_frame;

typedef struct s_manager
{
	void		*mlx;
	void		*window;
	t_ivec2		size;
	t_ivec2		cursor;
	int			click;
	t_ivec2		click_pos;
	t_frame		frame[2];
	int			curr_frame;
}	t_manager;

static int	init_frame(t_win *win, int index);
static void	set_hooks(t_win *win);

int	main(int argc, char **argv)
{
	t_win	win;

	bzero(&win, sizeof(t_win));
	win.mlx = mlx_init();
	if (!win.mlx)
		return (1);
	if (!set_sprite_array(&win, "img/index.json")
		|| !set_map_and_player(&win, argc, argv[1])
		|| !create_window(&win, RES_WIDTH, RES_HEIGHT))
	{
		dprintf(2, "Error: Failure during initialization\n");
		release_resources(&win);
		return (1);
	}
	add_outline_to_font(&win.sprites[2]);
	mlx_loop(win.mlx);
	return (0);
}

/*
	I had to remove `mlx_mouse_hide(win->mlx, win->win)` because it was 
	causing a leak.
*/
int	create_window(t_win *win, int width, int height)
{
	set_ivec2(&win->size, width, height);
	win->win = mlx_new_window(win->mlx, win->size.x, win->size.y, TITLE);
	if (!win->win)
		return (0);
	if (!init_frame(win, 0) || !init_frame(win, 1))
		return (0);
	set_hooks(win);
	return (1);
}

static int	init_frame(t_win *win, int index)
{
	t_frame	*frame;

	frame = &win->frame[index];
	set_ivec2(&frame->size, win->size.x, win->size.y);
	frame->thickness = frame->size.x / RES_WIDTH;
	frame->img = mlx_new_image(win->mlx, frame->size.x, frame->size.y);
	if (!frame->img)
		return (0);
	frame->addr = mlx_get_data_addr(frame->img, &frame->bpp,
			&frame->line_length, &frame->endian);
	return (1);
}

static void	set_hooks(t_win *win)
{
	mlx_hook(win->win, 17, 0L, on_close, win);
	mlx_hook(win->win, 4, 1L << 2, on_mouse_button, win);
	mlx_key_hook(win->win, on_key_press, win);
	mlx_loop_hook(win->mlx, render_loop, win);
	return ;
}

void	release_resources(t_win *win)
{
	if (win->win)
		mlx_destroy_window(win->mlx, win->win);
	if (win->frame[0].img)
		mlx_destroy_image(win->mlx, win->frame[0].img);
	if (win->frame[1].img)
		mlx_destroy_image(win->mlx, win->frame[1].img);
	mlx_destroy_display(win->mlx);
	free(win->mlx);
	return ;
}

int	render_loop(t_win *win)
{
	long	dt;
	t_frame	*frame;

	dt = get_delta_time();
	frame = &win->frame[win->curr_frame];
	bzero(frame->addr, frame->size.x * frame->size.y * (frame->bpp / 8));
	//render_game(win, frame, dt);
	on_mouse_move(win);
	win->curr_frame = (win->curr_frame + 1) % 2;
	mlx_put_image_to_window(win->mlx, win->win, frame->img, 0, 0);
	return (0);
}
