#include "ShittyMath.h"

float DegToRad(float deg)
{
	return deg * (PI / 180);
}

float RadToDeg(float rad)
{
	return rad * (180.0 / PI);
}

RayHit ShootRay(float px, float py, float rayDirX, float rayDirY, Map* map, int checkEntities,int checkPlayer)
{
	RayHit rayHit;
	rayHit.numEntityHits = 0;
	rayHit.wallType = -1;
	rayHit.player = NULL;
	float closestEntity = 100000000;

	int mapX = (int)px;
	int mapY = (int)py;
	//Ray stuff
	//Distance to next bound
	double sideDistX;
	double sideDistY;
	//Length from x to next x ||  y to next y
	double deltaDistX = fabs(1 / rayDirX);
	double deltaDistY = fabs(1 / rayDirY);
	double perpWallDist;
	//which dir to step
	int stepX;
	int stepY;
	int hit = 0;//did it hit a wall
	int side;//NS/EW

	// == Calc step and side dist == \\ 
	if (rayDirX < 0)
	{
		stepX = -1;
		sideDistX = (px - mapX) * deltaDistX;
	}
	else
	{
		stepX = 1;
		sideDistX = (mapX + 1.0 - px) * deltaDistX;
	}
	if (rayDirY < 0)
	{
		stepY = -1;
		sideDistY = (py - mapY) * deltaDistY;
	}
	else
	{
		stepY = 1;
		sideDistY = (mapY + 1.0 - py) * deltaDistY;
	}
	//DDA 
	while (hit == 0)
	{
		//jump to next map square, either in x-direction, or in y-direction
		if (sideDistX < sideDistY)
		{
			sideDistX += deltaDistX;
			mapX += stepX;
			side = 0;
		}
		else
		{
			sideDistY += deltaDistY;
			mapY += stepY;
			side = 1;
		}
		ResourceManager* rm = (ResourceManager*)resourceManager;

		//Check if ray has hit a wall
		if (map->layout[mapY * map->width + mapX] != '.')  hit = 1;

		if (checkEntities)
		{
			//LOOK FOR ENTITIES
			for (int i = 0; i < rm->numEntities; i++)
			{
				Entity* e = rm->entities[i];
				if (e->solid == 0)
					continue;
				int ex = (int)e->x;
				int ey = (int)e->y;

				if (ex == mapX && ey == mapY)
				{
					rayHit.entitiyHits[rayHit.numEntityHits] = e;
					rayHit.numEntityHits++;
				}
			}
		}
		if (checkPlayer)
			if (mapX == (int)rm->player->x && mapY == (int)rm->player->y)
				rayHit.player = rm->player;
	}
	return rayHit;
}