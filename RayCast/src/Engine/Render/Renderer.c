#include "Renderer.h"


void DrawMap3D(Map* map, Player* player, ResourceManager* rm, float* zBuffer)
{
	DrawFloors3D(map->layout, player, rm);
	DrawWalls3D(map, player, rm, zBuffer);
	DrawSprites(map->layout, player, rm, zBuffer);
}



void DrawWalls3D(Map* map, Player* player, ResourceManager* rm, float* zBuffer)
{
	for (int r = 0; r < screenWidth; r++)
	{
		//easier to type player vars
		float a = player->angle;
		float px = player->x;
		float py = player->y;
		float planeX = player->planeX;
		float planeY = player->planeY;
		int mapX = (int)px;
		int mapY = (int)py;
		//ray origin and direction
		double cameraX = 2 * r / (double)screenWidth - 1; 
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
		char c;
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
			c = map->layout[mapY * map->width + mapX];
			if (c != '.' )  hit = 1;
			if (c == '\0')return;
		}

		if (side == 0) perpWallDist = (sideDistX - deltaDistX);
		else          perpWallDist = (sideDistY - deltaDistY);

		zBuffer[r] = perpWallDist;

		//Calculate height of line to draw on screen
		int h = screenHeight;

		//Calculate height of line to draw on screen
		int lineHeight = (int)(h / perpWallDist);

		//calculate lowest and highest pixel to fill in current stripe
		int drawStart = -lineHeight / 2 + h / 2;
		if (drawStart < 0)drawStart = 0;
		int drawEnd = lineHeight / 2 + h / 2;
		if (drawEnd >= h)drawEnd = h - 1;

		Texture* t = GetWallTexture(c);
		int* wallTex = t->data; //rm->textures[0]->data;


		// === TEXTURING === \\ 
		//Get Wall's X
		double wallX; //where exactly the wall was hit
		if (side == 0) wallX = py + perpWallDist * rayDirY;
		else           wallX = px + perpWallDist * rayDirX;
		wallX -= floor((wallX));
		//Get X Coord on Texture
		int texX = (int)(wallX * 32.0);
		if (side == 0 && rayDirX > 0) texX = 32 - texX - 1;
		if (side == 1 && rayDirY < 0) texX = 32 - texX - 1;

		//Heigh tper pixel
		double step = 1.0 * 32 / lineHeight;
		double texPos = (drawStart - h / 2 + lineHeight / 2) * step;
		for (int y = drawStart; y < drawEnd; y++)
		{
			// Cast the texture coordinate to integer, and mask with (texHeight - 1) in case of overflow
			int texY = (int)texPos & (32 - 1);
			texPos += step;
			int color = wallTex[(31 - texY) * 32 + texX];

			//divide color by 2
			if (side == 1) color = (color >> 1) & 8355711;
			DrawPixel(r, y, color);
		}
	}
}
void DrawSprites(char* map, Player* player, ResourceManager* rm, float* zBuffer)
{
	int numSprites = rm->numEntities;

	int spriteDistances[MAX_SPRITES];

	for (int i = 0; i < numSprites; i++)
	{
		Entity* e = rm->entities[i];
		spriteDistances[i] = ((player->y - e->y) * (player->y - e->y)) + ((player->x - e->x) * (player->x - e->x));
	}

	//SORTING SPRITES
	int i, j;
	for (i = 0; i < numSprites - 1; i++)
		// Last i elements are already in place  
		for (j = 0; j < numSprites - i - 1; j++)
			if (spriteDistances[j] < spriteDistances[j + 1])
			{
				Entity* temp = rm->entities[j];
				rm->entities[j] = rm->entities[j + 1];
				rm->entities[j + 1] = temp;

				int tempI = spriteDistances[j];
				spriteDistances[j] = spriteDistances[j + 1];
				spriteDistances[j + 1] = tempI;

			}
	//project sprites
	for (int i = 0; i < rm->numEntities; i++)
	{
		Entity* entity = rm->entities[i];
		//Sprite Scaling 
		int uDiv = rm->entities[i]->uDiv;
		int vDiv = rm->entities[i]->vDiv;
		float vMove = rm->entities[i]->vMove;

		//translate sprite position to relative to camera
		float spriteX = rm->entities[i]->x - player->x;
		float spriteY = rm->entities[i]->y - player->y;

		//2x2 matrix go brrrr
		double invDet = 1.0 / (player->planeX * player->dirY - player->dirX * player->planeY);

		double transformX = invDet * (player->dirY * spriteX - player->dirX * spriteY);
		double transformY = invDet * (-player->planeY * spriteX + player->planeX * spriteY);

		int vMoveScreen = (int)(vMove / transformY);


		int spriteScreenX = (int)((screenWidth / 2) * (1 + transformX / transformY));
		//Calculate height 
		int spriteHeight = (int)abs((screenHeight / (transformY))) / vDiv;
		int drawStartY = -spriteHeight / 2 + screenHeight / 2 + vMoveScreen;
		if (drawStartY < 0) drawStartY = 0;
		int drawEndY = spriteHeight / 2 + screenHeight / 2 + vMoveScreen;
		if (drawEndY >= screenHeight) drawEndY = screenHeight - 1;

		//calculate width of the sprite
		int spriteWidth = (int)abs(screenHeight / (transformY)) / uDiv;
		int drawStartX = -spriteWidth / 2 + spriteScreenX;
		if (drawStartX < 0)
			drawStartX = 0;
		int drawEndX = spriteWidth / 2 + spriteScreenX;
		if (drawEndX >= screenWidth)
			drawEndX = screenWidth - 1;
		//loop through every vertical stripe of the sprite on screen
		for (int stripe = drawStartX; stripe < drawEndX; stripe++)
		{
			int texX = (int)(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * entity->width / spriteWidth) / 256;

			if (transformY > 0 && stripe > 0 && stripe < screenWidth && transformY < zBuffer[stripe])
				for (int y = drawStartY; y < drawEndY; y++)
				{
					int d = (y - vMoveScreen) * 256 - screenHeight * 128 + spriteHeight * 128; //help avoid floating point
					int texY = ((d * entity->height) / spriteHeight) / 256;
					//Flip Y
					int color = entity->texture->data[(entity->height - texY - 1) * entity->width + texX];
					if (color != GetRGB(0, 255, 255))//if not do not draw color
						DrawPixel(stripe, y, color);
				}
		}
	}

}

void DrawFloors3D(char* map, Player* player, ResourceManager* rm)
{
	//FLOOR CASTING
	for (int y = 64; y < screenWidth; y++)
	{
		float rayDirX0 = player->dirX - player->planeX;
		float rayDirY0 = player->dirY - player->planeY;
		float rayDirX1 = player->dirX + player->planeX;
		float rayDirY1 = player->dirY + player->planeY;

		int p = y - screenHeight / 2;

		float posZ = 0.5 * screenHeight;

		float rowDistance = posZ / p;

		float floorStepX = rowDistance * (rayDirX1 - rayDirX0) / screenWidth;
		float floorStepY = rowDistance * (rayDirY1 - rayDirY0) / screenWidth;

		float floorX = player->x + rowDistance * rayDirX0;
		float floorY = player->y + rowDistance * rayDirY0;

		for (int x = 0; x < screenWidth; ++x)
		{
			int cellX = (int)(floorX);
			int cellY = (int)(floorY);

			int tx = (int)(32 * (floorX - cellX)) & (32 - 1);
			int ty = (int)(32 * (floorY - cellY)) & (32 - 1);

			floorX += floorStepX;
			floorY += floorStepY;

			int color;

			color = rm->textures[2]->data[32 * ty + tx];
			color = (color >> 1) & 8355711; // darkens texture
			DrawPixel(x, y, color);

			color = rm->textures[1]->data[32 * ty + tx];
			color = (color >> 1) & 8355711; // Darken
			DrawPixel(x, screenHeight - y - 1, color);
		}
	}
}

//Draw debug map
#if 0
void DrawMap(char* map, Player* player)
{
	//DRAWS MAP 2D
	int cellSize = 20;
	//Draws Map
	int gridX = GetWidth() / 2 / cellSize;
	int gridY = GetHeight() / cellSize;

	for (int y = 0; y < cellSize; y++)
		for (int x = 0; x < cellSize; x++)
		{
			if (map[y * cellSize + x] != ' ')
				DrawSquare(x * gridX, y * gridY, gridX, gridY, GetRGB(0, 0, 255));
		}

	DrawSquare(player->x * gridX - (gridX / 2), player->y * gridY - (gridX / 2), gridX, gridY, GetRGB(0, 255, 0));


	float x1 = player->x * gridX;
	float y1 = player->y * gridY;

	float x2 = x1 + (player->dirX) * gridX * 3;
	float y2 = y1 + (player->dirY) * gridY * 3;

	DrawLine(x1, y1, x2, y2, GetRGB(255, 255, 255));

}
#endif

Texture* GetWallTexture(char c)
{
		
	ResourceManager* rm = (ResourceManager*) resourceManager;
	switch (c)
	{
	case 's'://Stone Wall
		return rm->textures[0];
		break;
	case 'w'://Wood Wall
		return rm->textures[24];
		break;
	default:
		return rm->textures[0];
		break;
	}
}