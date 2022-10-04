#pragma once
#include <math.h>
#include "ResourceManager.h"

typedef struct RayHit
{
	Entity* entitiyHits[128];
	int numEntityHits;

	int wallType;
	Player* player;

}RayHit;

#define PI 3.14159265358979323846
 float DegToRad(float deg);
 float RadToDeg(float rad);

 RayHit ShootRay(float px, float py, float rayDirX, float rayDirY, Map* map,int hitEntity,int hitPlayer);
