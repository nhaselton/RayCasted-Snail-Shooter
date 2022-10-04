#pragma once
#include <math.h>
#include "../../Engine/ResourceManager.h"
#include "../../Engine/Render/Texture.h"
#include "../../Engine/Render/Animation/Animation.h"
#include "../../Engine/ShittyMath.h"
#include "Enemy.h"
//
//TODO Implement
typedef struct SnailData
{
	float TODO;
}SnailData;

Entity* CreateSnail(ResourceManager* rm, int x, int y,Map* map);
void CreateSnailAnimations(ResourceManager* rm);
int CheckEntity(Entity* entity, float radius);

void UpdateSnail(Entity* snail,float px,float py);
//States
void SnailIdle(Entity* snail, float px, float py, ANIM_STATUS aStatus);
void SnailChasePlayer(Entity* entity,  float px,float py, ANIM_STATUS aStatus);
void SnailAttackPlayer(Entity* entity, float px,float py, ANIM_STATUS aStatus);
void ChangeSnailState(Entity* snail, ENEMY_STATE newState);
void SnailTakeDamage(Entity* snail, float damage);