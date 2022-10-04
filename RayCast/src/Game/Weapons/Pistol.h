#pragma once
#include "Weapon.h"
#include "../../Engine/ShittyMath.h"
#include "../../Engine/ResourceManager.h"

void CreatePistolAnimations(ResourceManager* rm);
Weapon* InitPistol(ResourceManager* rm);
void ShootPistol(Player* player, Map* map);