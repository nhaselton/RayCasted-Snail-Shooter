#pragma once
#include "../Engine/Render/Texture.h"
#include "../Engine/Render/Animation/Animation.h"
#include "../Tools/Helper.h"
//#include "Player.h"
#include "Enemies/Enemy.h"
#include "Map.h"

typedef struct Entity
{
	Texture* texture;
	float x;
	float y;
	int uDiv;//width
	int vDiv;//height
	float vMove;//vertical offset
	int visible;
	int width;
	int height;
	int solid;

	ENEMY_STATE state;
	Map* map;
	Animator* animator;
	
	//Functions
	void* update;
	void* changeState;
	void* PlayerTakeDamage;

	//Entity Type Data
	void* CustomEntityData;
	
} Entity;

void UpdateStatic(Entity* entity, float px,float py);
void ChangeStateStatic(Entity* entity, ENEMY_STATE newState);
void ChangeAnimation(Entity* entity,Animation* animation);