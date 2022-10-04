#pragma once
#include "../Engine/Global.h"
#include <windows.h>
#include <math.h>
#include "Map.h"
#include "../Tools/Helper.h"
#include "Weapons/Weapon.h"

typedef struct Player
{
	float x;
	float y;
	float angle;
	float dirX;
	float dirY;
	float planeX;
	float planeY;
	int hp;
	int ammo;
	Weapon* currentWeapon;
}Player;

void GetInput(Player* player,Map* map);
void ChangePlayerWeaponAnimation(Animator* animator, Animation* animation);
void UpdatePlayer(Player* player,Map* map);
void PlayerTakeDamage(Player* player, float damage);