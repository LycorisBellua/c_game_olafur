#include "cimmerian.h"

void	cast_floor_and_ceiling(t_frame *f, t_map *m)
{
	for (int y = f->size.y / 2 + 1; y < f->size.y; ++y)
	{
		// rayDir for leftmost ray (x = 0) and rightmost ray (x = w)
		double rayDirX0 = g_man.player.dir.x - g_man.player.plane.x;
		double rayDirY0 = g_man.player.dir.y - g_man.player.plane.y;
		double rayDirX1 = g_man.player.dir.x + g_man.player.plane.x;
		double rayDirY1 = g_man.player.dir.y + g_man.player.plane.y;

		// Current y position compared to the center of the screen (the horizon)
		int p = y - f->size.y / 2 + 1;

		// Vertical position of the camera.
		// NOTE: with 0.5, it's exactly in the center between floor and ceiling,
		// matching also how the walls are being raycasted. For different values
		// than 0.5, a separate loop must be done for ceiling and floor since
		// they're no longer symmetrical.
		double posZ = 0.5 * f->size.y;

		// Horizontal distance from the camera to the floor for the current row.
		// 0.5 is the z position exactly in the middle between floor and ceiling.
		// NOTE: this is affine texture mapping, which is not perspective correct
		// except for perfectly horizontal and vertical surfaces like the floor.
		// NOTE: this formula is explained as follows: The camera ray goes through
		// the following two points: the camera itself, which is at a certain
		// height (posZ), and a point in front of the camera (through an imagined
		// vertical plane containing the screen pixels) with horizontal distance
		// 1 from the camera, and vertical position p lower than posZ (posZ - p). When going
		// through that point, the line has vertically traveled by p units and
		// horizontally by 1 unit. To hit the floor, it instead needs to travel by
		// posZ units. It will travel the same ratio horizontally. The ratio was
		// 1 / p for going through the camera plane, so to go posZ times farther
		// to reach the floor, we get that the total horizontal distance is posZ / p.
		double rowDistance = posZ / p;

		if (rowDistance > m->dof)
			continue ;

		// calculate the real world step vector we have to add for each x (parallel to camera plane)
		// adding step by step avoids multiplications with a weight in the inner loop
		double floorStepX = rowDistance * (rayDirX1 - rayDirX0) / f->size.x;
		double floorStepY = rowDistance * (rayDirY1 - rayDirY0) / f->size.x;

		// real world coordinates of the leftmost column. This will be updated as we step to the right.
		double floorX = g_man.player.pos.x + rowDistance * rayDirX0;
		double floorY = g_man.player.pos.y + rowDistance * rayDirY0;

		for (int x = 0; x < f->size.x; ++x)
		{
			// Stop rendering if outside the map
			if (!(floorX < 0 || floorX > m->size.x || floorY < 0 || floorY > m->size.y))
			{
				// the cell coord is simply got from the integer parts of floorX and floorY
				int cellX = (int)floorX;
				int cellY = (int)floorY;

				// floor
				int floorTexture = !((cellX + cellY) % 2) ? 4 : 6;
				int texWidth = m->img[floorTexture]->size.x;
				int texHeight = m->img[floorTexture]->size.y;
				int tx = (int)(texWidth * (floorX - cellX)) % texWidth;
				int ty = (int)(texHeight * (floorY - cellY)) % texHeight;
				t_color color;
				color = ((t_color *)m->img[floorTexture]->buf)[texWidth * ty + tx];
				color.r /= 2;
				color.g /= 2;
				color.b /= 2;
				apply_wall_fog(&color, m->fog_color, rowDistance, m->dof);
				draw_point(f, color, x, y);

				// ceiling
				int ceilingTexture = 7;
				texWidth = m->img[ceilingTexture]->size.x;
				texHeight = m->img[ceilingTexture]->size.y;
				tx = (int)(texWidth * (floorX - cellX)) % texWidth;
				ty = (int)(texHeight * (floorY - cellY)) % texHeight;
				color = ((t_color *)m->img[ceilingTexture]->buf)[texWidth * ty + tx];
				color.r /= 2;
				color.g /= 2;
				color.b /= 2;
				apply_wall_fog(&color, m->fog_color, rowDistance, m->dof);
				draw_point(f, color, x, f->size.y - y - 1);
			}

			floorX += floorStepX;
			floorY += floorStepY;
		}
	}
	return ;
}
