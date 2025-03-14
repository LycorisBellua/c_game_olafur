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
	t_ubyte	*addr; // was `char *`
	int		bpp;
	int		line_length;
	int		endian;
	int		thickness;
	t_ivec2	size;
}	t_frame;

typedef struct s_man
{
	void		*mlx;
	void		*window;
	t_ivec2		size;
	t_ivec2		cursor;
	int			click;
	t_ivec2		click_pos;
	t_frame		frame[2];
	int			curr_frame;
}	t_man;

static int	init_frame(t_man *man, int index);
static void	set_hooks(t_man *man);

int	main(int argc, char **argv)
{
	t_man	man;

	bzero(&man, sizeof(t_man));
	if (!create_window(&g_man, TITLE, RES_WIDTH, RES_HEIGHT))
		return (1);
	if (!init_frame(man, 0) || !init_frame(man, 1))
		return (0);
	set_hooks(man);
	if (!set_image_array(&man, "img/index.json") || !set_map_and_player(&man, argc, argv[1]))
	{
		dprintf(2, "Error: Failure during initialization\n");
		deinit(&man);
		return (1);
	}
	add_outline_to_font(&man.images[1]);
	mlx_loop(man.mlx);
	return (0);
}

static int	init_frame(t_man *man, int index)
{
	t_frame	*frame;

	frame = &man->frame[index];
	set_ivec2(&frame->size, man->res.window_size_default.x, man->res.window_size_default.y);
	set_ivec2(&frame->real_size, man->res.window_size.x, man->res.window_size.y);
	frame->thickness = frame->real_size.x / frame->size.x;
	frame->img = mlx_new_image(man->mlx, frame->real_size.x, frame->real_size.y);
	if (!frame->img)
		return (0);
	frame->addr = mlx_get_data_addr(frame->img, &frame->bpp,
			&frame->line_length, &frame->endian);
	return (1);
}

/*
	I had to remove `mlx_mouse_hide(man->mlx, man->window)` because it was 
	causing a leak.
*/
static void	set_hooks(t_man *man)
{
	mlx_hook(man->window, 17, 0L, on_close, man);
	mlx_hook(man->window, 4, 1L << 2, on_mouse_button, man);
	mlx_key_hook(man->window, on_key_press, man);
	mlx_loop_hook(man->mlx, render_loop, man);
	return ;
}

int	render_loop(t_man *man)
{
	long	dt;
	t_frame	*frame;

	dt = get_delta_time();
	frame = &man->frame[man->curr_frame];
	bzero(frame->addr, frame->real_size.x * frame->real_size.y * (frame->bpp / 8));
	//render_game(man, frame, dt);
	on_mouse_move(man);
	man->curr_frame = (man->curr_frame + 1) % 2;
	mlx_put_image_to_window(man->mlx, man->man, frame->img, 0, 0);
	return (0);
}
