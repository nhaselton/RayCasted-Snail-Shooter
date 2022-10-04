#pragma once
#include "../../Engine/Render/Animation/Animation.h"


typedef enum WEAPON_STATE
{
	WEAPON_IDLE,
	WEAPON_SHOOTING,
	WEAPON_COOLDOWN
}WEAPON_STATE;

typedef struct Weapon
{
	float cooldown;
	float maxCooldown;

	int drawX;
	int drawY;
	int scaleX;
	int scaleY;

	Animator* animator;
	//Functions
	void* Shoot;

	//can be NULL
	void* extraInfo;
	WEAPON_STATE state;
	
}Weapon;

void Shoot();
