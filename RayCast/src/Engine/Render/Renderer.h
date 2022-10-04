#pragma once
#include "../Global.h"
#include "../ResourceManager.h"
#include "../../game/Player.h"
#include "Draw.h"


void DrawMap(char* map, Player* player); 
void DrawMap3D(Map* map, Player* player, ResourceManager* rm,float* zBuffer);
void DrawWalls3D(Map* map, Player* player, ResourceManager* rm,float* zBuffer);
void DrawFloors3D(char* map, Player* player,ResourceManager* rm);
void DrawSprites(char* map, Player* player, ResourceManager* rm, float* zBuffer);
Texture* GetWallTexture(char c);
