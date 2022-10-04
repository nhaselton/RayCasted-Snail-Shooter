#include "Game.h"

/*TODO LIST (2014 lines of code)
	4) Enemy Collisions with each other (maybe do float collsion so they can be same square but sligtly far aprt


	ENEMY STUFF
	a) Cooldown before attakcing again?
	*/


Player player = { 2,2,0, 1,1,0.0f,.66f };	
float* zBuffer;
int enemyCount = 0;
Map map;
ResourceManager rm;
GAME_STATE state;



//TODO fix 
void InitGame()
{
	state = GAME_MENU;

	playerSpeed = 15.0f;
	playerTurnSpeed = 4.f;
	player.hp = 100;
	player.ammo = 100;

	//TODO maybe hashmap for string->texture 
	rm = CreateResourceManager();
	AddTextures(&rm);
	
	rm.player = &player;
	resourceManager = &rm;


	
	/* Animations
	* 0) Snail Chase
	* 1) Snail Attack
	* 2) Snail Pain
	* 3) Snail Death
	* 4) Pistol Idle
	* 5) Pistol Shoot
	* 6) 
	*/


	player.dirX = cos(player.angle);
	player.dirY = sin(player.angle);
	map = CreateMap("res/map/map.map",&rm);
	rm.currentMap = &map;
	//AddEntity(&rm, 6, 10, rm.textures[3]);
	zBuffer = malloc(screenWidth * sizeof(float));

	/*CREATE ANIM HERE*/
	int snail1 = 5;

	CreateSnailAnimations(&rm);

	CreateEnemies(&map,&rm);
	
	InitPistol(&rm);

	player.currentWeapon = rm.weapons[0];
	player.ammo = 200;
}


int cursorLoc = 0;
unsigned long nextMove = 4;

void UpdateGame()
{

	switch (state)
	{
	case GAME_MENU:
		
		DrawImage(0, 0, 640, 640, 1, 1, rm.textures[26]->data);
		
		if(cursorLoc == 0)
			DrawImage(150, 250, 32, 32, 1, 1, rm.textures[28]->data);
		else
			DrawImage(150, 360, 32, 32, 1, 1, rm.textures[28]->data);

		if ((GetKey(VK_UP) || GetKey(VK_DOWN)) && currentTime > nextMove)
		{
				cursorLoc = !cursorLoc;
				//Add a cooldwon
				nextMove = currentTime + (1500000);
		}
		if (GetKey(VK_RETURN))
		{
			if (cursorLoc == 0)
				state = GAME_PLAYING;
			else
				exit(0);
		}

		break;

	case GAME_PLAYING:
	{

		//INPUT
		GetInput(&player, &map);
		//PLAYER UPDATE
		UpdatePlayer(&player, &map);
		//ENTITY UPDATE

		int currentEnemies = 0;
		for (int i = 0; i < rm.numEntities; i++)
		{
			float px = player.x;
			float py = player.y;

			Entity* e = rm.entities[i];

			if (e != NULL)
			{
				EnemyCustomData* eD = (EnemyCustomData*)e->CustomEntityData;
				if (!eD->dead)
					currentEnemies++;
				void(*u)(Entity*, float, float) = e->update;
				u(e, px, py);
			}
		}
		enemyCount = currentEnemies;
		//UpdateSnail(rm.entities[0], player.x, player.y);

		DrawGame();
		Weapon* pistol = rm.weapons[0];

		DrawImage(pistol->drawX, pistol->drawY - 64, pistol->animator->currentFrame->width, pistol->animator->currentFrame->height, pistol->scaleX, pistol->scaleY, pistol->animator->currentFrame->data);

		DrawImage(0, bitmapHeight - 64, 640, 64, 1, 1, rm.textures[25]->data);
		DrawHud(&rm);
		if (enemyCount == 0)
			state = GAME_WIN;
		if (player.hp <= 0)
			state = GAME_DEAD;
									//until you can quit
	}
		break;
	
	case GAME_WIN:
		DrawImage(0, 0, 640, 640, 1, 1, rm.textures[29]->data);
		break;
	case GAME_DEAD:
		DrawImage(0, 0, 640, 640, 1, 1, rm.textures[27]->data);
		break;
	}

}


void DrawHud(ResourceManager* rm)
{

	//Draw HP
	DrawMultiNumber(60, bitmapHeight - 40, 2, 1, player.hp, GetRGB(255, 0, 0));
	DrawMultiNumber(320, bitmapHeight - 40, 2, 1, player.ammo, GetRGB(255, 0, 0));
	DrawMultiNumber(550, bitmapHeight-40, 2, 1, enemyCount, GetRGB(255, 0, 0));
}

void DrawGame()
{
	DrawMap3D(&map, &player, &rm, zBuffer);
}

void CreateEnemies(Map* map, ResourceManager* rm)
{
	for (int i = 0; i < map->width * map->height; i++)
	{
		if (map->layout[i] == 'S')
		{
			//set to clear
			map->layout[i] = '.';

			//42
			//
			int x = i % map->width;
			int y = i/map->width;
			CreateSnail(rm, x, y,map);
			enemyCount++;
		}
		
	}
}

void AddTextures(ResourceManager* rm)
{
	//Textures
	AddTexture(rm, "res/img/wall.bmp");	//0
	AddTexture(rm, "res/img/sky.bmp");		//1
	AddTexture(rm, "res/img/grass.bmp");	//2
	AddTexture(rm, "res/img/barrel.bmp");	//3
	AddTexture(rm, "res/img/bush.bmp");	//4

	//Snail Idle
	AddTexture(rm, "res/img/snail/Snail.bmp");		//5
	AddTexture(rm, "res/img/snail/Snail2.bmp");	//6
	AddTexture(rm, "res/img/snail/Snail3.bmp");	//7

	//Snail Attack
	AddTexture(rm, "res/img/snail/SnailA1.bmp");	//8
	AddTexture(rm, "res/img/snail/SnailA2.bmp");	//9
	AddTexture(rm, "res/img/snail/SnailA3.bmp");	//10
	AddTexture(rm, "res/img/snail/SnailA4.bmp");	//11

	//Pistol Idle
	AddTexture(rm, "res/img/Pistol/Pistol.bmp");		//12
	//Pistol Shoot
	AddTexture(rm, "res/img/Pistol/PistolShoot1.bmp");	//13
	AddTexture(rm, "res/img/Pistol/PistolShoot2.bmp");	//14
	AddTexture(rm, "res/img/Pistol/PistolShoot3.bmp");	//15

	//Snail Pain
	AddTexture(rm, "res/img/snail/SnailP1.bmp");	//16
	AddTexture(rm, "res/img/snail/SnailP2.bmp");	//17
	AddTexture(rm, "res/img/snail/SnailP3.bmp");	//18

	//Snail Death
	AddTexture(rm, "res/img/snail/SnailD1.bmp");	//19
	AddTexture(rm, "res/img/snail/SnailD2.bmp");	//20
	AddTexture(rm, "res/img/snail/SnailD3.bmp");	//21
	AddTexture(rm, "res/img/snail/SnailD4.bmp");	//22
	AddTexture(rm, "res/img/snail/SnailD5.bmp");	//23

	//More TExtures
	AddTexture(rm, "res/img/wood.bmp");	//24
	AddTexture(rm, "res/img/Hud.bmp");		//25

	AddTexture(rm, "res/img/Title.bmp"); //26
	AddTexture(rm, "res/img/Dead.bmp");  //27
	AddTexture(rm, "res/img/Cursor.bmp");  //28
	AddTexture(rm, "res/img/Win.bmp");  //29
}