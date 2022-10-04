#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "../Engine/ResourceManager.h"
#include "../engine/Win32.h"
#include "../engine/render/Draw.h"
#include "../game/Player.h"
#include "../engine/ShittyMath.h"
#include "../tools/Helper.h"
#include "../engine/render/Renderer.h"
#include "../Game/Entity.h"
#include "Enemies/Snail.h"
#include "../Engine/Render/Animation/Animation.h"

#include "Weapons/Weapon.h"
#include "Weapons/Pistol.h"
#include "Enemies/Snail.h"
#include "Enemies/Enemy.h"

typedef enum GAME_STATE
{
	GAME_MENU,
	GAME_PLAYING,
	GAME_DEAD,
	GAME_WIN
}GAME_STATE;

void AddTextures(ResourceManager* rm);
void InitGame();
void UpdateGame();
void DrawGame();
void CreateEnemies(Map* map,ResourceManager* rm);
void DrawHud(ResourceManager* rm);