#include "cimmerian.h"

static void	set_chalice_sprites(t_spr **spr, t_img *img);
static void	set_pillar_sprites(t_spr **spr, t_img *img);
static void	set_barrel_sprites(t_spr **spr, t_img *img);

int	set_sprite_array(t_man *man, int length)
{
	int	i;

	man->map->sprite_len = length;
	man->map->sprites = calloc(man->map->sprite_len, sizeof(t_spr *));
	if (!man->map->sprites)
		return (0);
	i = 0;
	while (i < man->map->sprite_len)
	{
		man->map->sprites[i] = calloc(1, sizeof(t_spr));
		if (!man->map->sprites[i])
			return (0);
		++i;
	}
	set_chalice_sprites(man->map->sprites, get_image(man, "chalice"));
	set_pillar_sprites(man->map->sprites, get_image(man, "pillar"));
	set_barrel_sprites(man->map->sprites, get_image(man, "barrel"));
	return (1);
}

void	free_sprite_array(t_man *man)
{
	int	i;

	if (!man->map)
		return ;
	if (man->map->sprites)
	{
		i = 0;
		while (i < man->map->sprite_len)
		{
			free(man->map->sprites[i]);
			++i;
		}
	}
	free(man->map->sprites);
	man->map->sprite_len = 0;
	return ;
}

static void	set_chalice_sprites(t_spr **spr, t_img *img)
{
	int	i;

	i = 0;
	while (i < 8)
	{
		spr[i]->img = img;
		++i;
	}
	set_vec2(&spr[0]->pos, 20.5, 11.5);
	set_vec2(&spr[1]->pos, 18.5, 4.5);
	set_vec2(&spr[2]->pos, 10.0, 4.5);
	set_vec2(&spr[3]->pos, 10.0, 12.5);
	set_vec2(&spr[4]->pos, 3.5, 6.5);
	set_vec2(&spr[5]->pos, 3.5, 20.5);
	set_vec2(&spr[6]->pos, 3.5, 14.5);
	set_vec2(&spr[7]->pos, 14.5, 20.5);
	return ;
}

static void	set_pillar_sprites(t_spr **spr, t_img *img)
{
	int	i;

	i = 8;
	while (i < 12)
	{
		spr[i]->img = img;
		++i;
	}
	set_vec2(&spr[8]->pos, 18.5, 10.5);
	set_vec2(&spr[9]->pos, 18.5, 11.5);
	set_vec2(&spr[10]->pos, 18.5, 12.5);
	set_vec2(&spr[11]->pos, 8.5, 7.0);
	return ;
}

static void	set_barrel_sprites(t_spr **spr, t_img *img)
{
	int	i;

	i = 12;
	while (i < 20)
	{
		spr[i]->img = img;
		++i;
	}
	set_vec2(&spr[12]->pos, 21.5, 1.5);
	set_vec2(&spr[13]->pos, 15.5, 1.5);
	set_vec2(&spr[14]->pos, 16.0, 1.8);
	set_vec2(&spr[15]->pos, 16.2, 1.2);
	set_vec2(&spr[16]->pos, 3.5, 2.5);
	set_vec2(&spr[17]->pos, 9.5, 15.5);
	set_vec2(&spr[18]->pos, 10.0, 15.1);
	set_vec2(&spr[19]->pos, 10.5, 15.8);
	return ;
}
