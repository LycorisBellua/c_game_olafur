#include "cimmerian.h"

typedef struct s_sprite
{
  double x;
  double y;
  int texture;
}	t_sprite;

#define numSprites 20
t_sprite sprite[numSprites] =
{
	//green lights
	{20.5, 11.5, 12},
	{18.5,  4.5, 12},
	{10.0,  4.5, 12},
	{10.0, 12.5, 12},
	{ 3.5,  6.5, 12},
	{ 3.5, 20.5, 12},
	{ 3.5, 14.5, 12},
	{14.5, 20.5, 12},

	//pillars
	{18.5, 10.5, 11},
	{18.5, 11.5, 11},
	{18.5, 12.5, 11},
	{ 8.5,  7.0, 11},

	//barrels
	{21.5,  1.5, 10},
	{15.5,  1.5, 10},
	{16.0,  1.8, 10},
	{16.2,  1.2, 10},
	{ 3.5,  2.5, 10},
	{ 9.5, 15.5, 10},
	{10.0, 15.1, 10},
	{10.5, 15.8, 10},
};

static void sortSprites(int *order, double *dist, int amount);
static void	render_sprite(t_frame *f, t_map *m, t_sprite *s, double gridDistance, double *z_buffer);

void	cast_sprites(t_frame *f, t_map *m, double *z_buffer)
{
	//arrays used to sort the sprites
	int spriteOrder[numSprites];
	double spriteDistance[numSprites];

	for (int i = 0; i < numSprites; ++i)
	{
		spriteOrder[i] = i;
		spriteDistance[i] = get_squared_dist_euclidean(g_man.player.pos.x, g_man.player.pos.y, sprite[i].x, sprite[i].y);
	}
	sortSprites(spriteOrder, spriteDistance, numSprites);

	//after sorting the sprites, do the projection and draw them
	for (int i = 0; i < numSprites; ++i)
	{
		double gridDistance = f_sqrt(spriteDistance[i]);
		if (gridDistance > m->dof)
			continue ;
		render_sprite(f, m, sprite + spriteOrder[i], gridDistance, z_buffer);
	}
	return ;
}

static void sortSprites(int *order, double *dist, int amount)
{
	for (int i = 0; i < amount - 1; ++i)
	{
		for (int j = 0; j < amount - i - 1; ++j)
		{
			// Compare distances
			if (dist[j] < dist[j + 1])
			{
				// Swap distances
				double tempDist = dist[j];
				dist[j] = dist[j + 1];
				dist[j + 1] = tempDist;

				// Swap orders
				int tempOrder = order[j];
				order[j] = order[j + 1];
				order[j + 1] = tempOrder;
			}
		}
	}
	return ;
}

static void	render_sprite(t_frame *f, t_map *m, t_sprite *s, double gridDistance, double *z_buffer)
{
	//translate sprite position to relative to camera
	double spriteX = s->x - g_man.player.pos.x;
	double spriteY = s->y - g_man.player.pos.y;

	//transform sprite with the inverse camera matrix
	// [ planeX   dirX ] -1                                       [ dirY      -dirX ]
	// [               ]       =  1/(planeX*dirY-dirX*planeY) *   [                 ]
	// [ planeY   dirY ]                                          [ -planeY  planeX ]

	//required for correct matrix multiplication
	double invDet = 1.0 / ((g_man.player.plane.x * g_man.player.dir.y - g_man.player.dir.x * g_man.player.plane.y) * g_man.res.h_mod);

	double transformX = invDet * (g_man.player.dir.y * spriteX - g_man.player.dir.x * spriteY);
	//this is actually the depth inside the screen, that what Z is in 3D, the distance of sprite to player, matching sqrt(spriteDistance[i])
	double transformY = invDet * (-g_man.player.plane.y * spriteX + g_man.player.plane.x * spriteY);

	int spriteScreenX = (int)((f->size.x / 2) * (1 + transformX / transformY));

	//parameters for scaling and moving the sprites
	const int uDiv = 1;
	const int vDiv = 1;
	const double vMove = 0.0;
	int vMoveScreen = (int)(vMove / transformY);

	//calculate height of the sprite on screen
	int spriteHeight = abs((int)(f->size.y / transformY)) / vDiv; //using "transformY" instead of the real distance prevents fisheye
																  //calculate lowest and highest pixel to fill in current stripe
	int drawStartY = max(-spriteHeight / 2 + f->size.y / 2 + vMoveScreen, 0);
	int drawEndY = min(spriteHeight / 2 + f->size.y / 2 + vMoveScreen, f->size.y - 1);

	//calculate width of the sprite
	int spriteWidth = abs((int)(f->size.y / transformY)) / uDiv; // same as height of sprite, given that it's square
	int drawStartX = max(-spriteWidth / 2 + spriteScreenX, 0);
	int drawEndX = min(spriteWidth / 2 + spriteScreenX, f->size.x);

	//loop through every vertical stripe of the sprite on screen
	for (int stripe = drawStartX; stripe < drawEndX; ++stripe)
	{
		int texWidth = m->img[s->texture]->size.x;
		int texHeight = m->img[s->texture]->size.y;
		/*
		//TEST
		if (s->texture == 8)
		{
			texWidth = g_man.sprites[2].size.x; 
			texHeight = g_man.sprites[2].size.y; 
		}
		*/

		int texX = (int)(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * texWidth / spriteWidth) / 256;
		//the conditions in the if are:
		//1) it's in front of camera plane so you don't see things behind you
		//2) z_buffer, with perpendicular distance
		if (transformY > 0 && transformY < z_buffer[stripe] / g_man.res.h_mod)
		{
			for (int y = drawStartY; y < drawEndY; ++y) //for every pixel of the current stripe
			{
				int d = (y - vMoveScreen) * 256 - f->size.y * 128 + spriteHeight * 128; //256 and 128 factors to avoid floats
				int texY = ((d * texHeight) / spriteHeight) / 256;
				//get current color from the texture
				t_color color = ((t_color *)m->img[s->texture]->buf)[texWidth * texY + texX];
				/*
				//TEST
				if (s->texture == 8)
					color = g_man.sprites[2].cycle[g_man.sprites[2].cycle_index][texWidth * texY + texX];
				*/
				apply_wall_fog(&color, m->fog_color, gridDistance, m->dof);
				draw_point(f, color, stripe, y);
			}
		}
	}
	return ;
}
