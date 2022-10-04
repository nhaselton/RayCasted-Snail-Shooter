#include "Pistol.h"

Weapon* InitPistol(ResourceManager* rm)
{
	CreatePistolAnimations(rm);
	Weapon* pistol = AddWeapon(rm);


	pistol->drawX = bitmapWidth / 2 - 128;
	pistol->drawY = bitmapHeight - (64 * 4);
	pistol->scaleX = 4;
	pistol->scaleY = 4;

	//Animator Garbaeg
	pistol->animator = (Animator*) malloc(sizeof(Animator));
	pistol->animator->animations = malloc(2 * sizeof(Animation*));
	pistol->animator->animations[0] = rm->animations[4];
	pistol->animator->animations[1] = rm->animations[5];
	pistol->animator->currentAnimTime = 0;
	pistol->animator->currentFrameTime = 0;
	pistol->animator->currentFrameIndex = 0;
	pistol->animator->currentAnimation = pistol->animator->animations[0];
	pistol->animator->currentFrame = pistol->animator->animations[0]->frames[0];
	pistol->Shoot = ShootPistol;

}




void CreatePistolAnimations(ResourceManager* rm)
{
	Animation* idle = (Animation*) malloc(sizeof(Animation));
	idle->numFrames = 1;
	idle->duration = 1;
	idle->frames = malloc(idle->numFrames * sizeof(Texture*));
	idle->frames[0] = rm->textures[12];
	idle->frameLengths = malloc(sizeof(float));
	idle->frameLengths[0] = 1;

	AddAnimation(rm, idle);
	
	Animation* shoot = (Animation*)malloc(sizeof(Animation));
	shoot->numFrames = 3;
	shoot->duration = 0;
	shoot->frames = malloc(shoot->numFrames * sizeof(Texture*));
	
	int shootFrame = 12;
	shoot->frames[0] = rm->textures[shootFrame+0];
	shoot->frames[1] = rm->textures[shootFrame+1];
	shoot->frames[2] = rm->textures[shootFrame+2];
	
	shoot->frameLengths = malloc(3 * sizeof(float));
	shoot->frameLengths[0] = .1f;
	shoot->frameLengths[1] = .2f;
	shoot->frameLengths[2] = .2f;
	
	shoot->numFrameTriggers = 1;
	shoot->frameTriggers = malloc(sizeof(int));
	shoot->frameTriggers[0] = 1;

	for (int i = 0; i < shoot->numFrames; i++)
		shoot->duration += shoot->frameLengths[i];
	
	AddAnimation(rm, shoot);
}

void ShootPistol(Player* player, Map* map)
{

	Entity* kill = NULL;
	float closest = 1000000;

	ResourceManager* rm = (ResourceManager*)resourceManager;

	//Ray stuff
	double cameraX = 2 * (bitmapWidth / 2) / (double)screenWidth - 1; //MIDDLE OF SCREEN
	float rayDirX = player->dirX + player->planeX * cameraX;
	float rayDirY = player->dirY + player->planeY * cameraX;


	RayHit hit = ShootRay(player->x, player->y, rayDirX, rayDirY, map,1,0);
	
	for (int i = 0; i < hit.numEntityHits; i++)
	{
		Entity* e = hit.entitiyHits[i];
		int ex = (int)e->x;
		int ey = (int)e->y;

		{
			float distance = sqrtf((player->x- ex) * (player->x - ex)) + ((player->y - ey) * (player->y - ey));
			if (distance < closest)
			{
				kill = e;
				closest = distance;
			}
		}
	}
	if (kill != NULL)
		if (kill->PlayerTakeDamage != NULL)
		{
			void(*u)(Entity*, float) = kill->PlayerTakeDamage;
			u(kill, 1);
		}

	/*

	//SHOOTS A RAY 
	float px = player->x;
	float py = player->y;
	float planeX = player->planeX;
	float planeY = player->planeY;
	int mapX = (int)px;
	int mapY = (int)py;
	//Ray stuff
	double cameraX = 2 * (bitmapWidth/2) / (double)screenWidth - 1; //MIDDLE OF SCREEN
	float rayDirX = player->dirX + planeX * cameraX;
	float rayDirY = player->dirY + planeY * cameraX;
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
		//Check if ray has hit a wall
		if (map->layout[mapY * map->width + mapX] == '.')  hit = 1;
		//if (map->layout[mapY * map->width + mapX] == '\0') return;
		//CHECK WITH ENTITIES
		//TODO BSP?
		for (int i = 0; i < rm->numEntities; i++)
		{
			Entity* e = rm->entities[i];
			int ex = (int)e->x;
			int ey = (int)e->y;

			if (ex == mapX && ey == mapY)
			{
				float distance = ((px - ex)  * (px-ex)) + ((py-ey) * (py-ey));
				if (distance < closest)
				{
					kill = e;
					closest = distance;
				}
				//DeleteEntity(rm, e);
			}
		}
	}
	if (kill != NULL)
	{
		if (kill->TakeDamage != NULL)
		{
			void(*u)(Entity*, float) = kill->TakeDamage;
			u(kill, 1);
		}
		//DeleteEntity(rm, kill);
	}
	*/
}
