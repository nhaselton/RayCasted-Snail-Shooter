#include "Snail.h"


Entity* CreateSnail(ResourceManager* rm, int x, int y,Map* map)
{
	Entity* snail = AddEntityCustomSize(rm, x, y, rm->textures[5], 2, 2, 2 * 64);
	snail->animator = (Animator*)malloc(sizeof(Animator));

	snail->animator = malloc(sizeof(Animator));
	snail->animator->animations = malloc(sizeof(Animation*) *10);
	snail->animator->animations[0] = rm->animations[0];
	snail->animator->animations[1] = rm->animations[1];
	snail->animator->animations[2] = rm->animations[2];
	snail->animator->animations[3] = rm->animations[3];
	snail->animator->currentAnimTime = 0;
	snail->animator->currentFrameTime = 0;
	snail->animator->currentFrameIndex = 0;
	snail->animator->currentAnimation = rm->animations[0];
	snail->animator->currentFrame = rm->animations[0]->frames[0];
	//functions
	snail->update = UpdateSnail;
	snail->changeState = ChangeSnailState;

	snail->state = E_IDLE;
	snail->map = map;
	snail->PlayerTakeDamage = SnailTakeDamage;

	snail->CustomEntityData = malloc(sizeof(EnemyCustomData));
	EnemyCustomData* data = (EnemyCustomData*) snail->CustomEntityData;
	data->dead = 0;
	data->hp = 2;


	return snail;
}

void UpdateSnail(Entity* snail,float px,float py)
{
	EnemyCustomData* data = (EnemyCustomData*)snail->CustomEntityData;
	if (data->dead)
		return;

	ANIM_STATUS aStatus = UpdateAnimator(snail->animator);
	
	snail->texture = GetTexture(snail->animator);
	switch (snail->state)
	{
	case E_NONE:
		break;
	case E_IDLE:
		SnailIdle(snail, px, py, aStatus);
		break;
	case E_CHASE:
		SnailChasePlayer(snail, px,py,aStatus);
		break;
	case E_ATTACK:
		SnailAttackPlayer(snail,px,py,aStatus);
		break;
	case E_PAIN:
		if (aStatus == ANIM_FINISHED) ChangeSnailState(snail, E_CHASE);
		break;
	case E_DYING:
		snail->solid = 0;
		if (aStatus == ANIM_FINISHED) ChangeSnailState(snail, E_DEAD);
		break;
	}
	//Animation* a = snail->animator->currentAnimation;
	//snail->texture = a->frames[snail->animator->currentFrame];
	
}

void SnailIdle(Entity* snail,float px,float py,ANIM_STATUS aStatus)
{
	ResourceManager* rm = (ResourceManager*)resourceManager;

	float dist = sqrtf((px - snail->x) * (px - snail->x) + (py - snail->y) * (py - snail->y));
	if (dist < 10)
	{
		float dx = (px-snail->x)/dist;
		float dy = (py-snail->y)/dist;


		if(ShootRay(snail->x,snail->y,dx,dy,rm->currentMap,0,1).player != NULL)
		ChangeSnailState(snail, E_CHASE);
	}
}

void SnailChasePlayer(Entity* snail, float px, float py,ANIM_STATUS aStatus)
{
	ResourceManager* rm = (ResourceManager*)resourceManager;

	// ----- MOVE -------- \\ 
	//get distance
	float dX = (px - snail->x);
	float dY = (py - snail->y);
	//normalzie
	float c =  sqrtf(((dX * dX) + (dY * dY)));
	dX /= c;
	dY /= c;

	float mX = dX * deltaTime * 2;
	float mY = dY * deltaTime * 2;
	if (CheckFree(snail->map, snail->x + mX, snail->y))
	{
		snail->x += mX;
	}
	if (CheckFree(snail->map, snail->x , snail->y+mY))
	{
		snail->y += mY;
	}

	//PLEASE GOD FIX THIS!
	//CHECK COLLISIONS
	for (int i = 0; i < rm->numEntities; i++)
	{
		Entity* e = rm->entities[i];
		//SKIP IF SAME
		if (e == snail || e->solid == 0)
			continue;

		float x2 = rm->entities[i]->x;
		float y2 = rm->entities[i]->y;

		float dist = sqrtf(((snail->x - x2) * (snail->x - x2)) + ((snail->y - y2) * (snail->y - y2)));
		
		if (dist <= .5f)
		{
			float angle = atan2f(snail->x - x2, snail->y - y2);
			float mx = sin(angle)*deltaTime;
			float my = cos(angle)*deltaTime;

			if (CheckFree(snail->map, snail->x + mX, snail->y))
			{
				snail->x -= mX;
			}
			if (CheckFree(snail->map, snail->x, snail->y + mY))
			{
				snail->y -= mY;
			}
		}
	}


	//SWAP STATE IF CLOSE
	if (c <= .75f)
	{
		ChangeSnailState(snail,E_ATTACK);
	}

	//go back to idle
	if (c > 11)
		ChangeSnailState(snail, E_IDLE);
}

void SnailAttackPlayer(Entity* snail, float px, float py, ANIM_STATUS aStatus)
{
	ResourceManager* rm = (ResourceManager*)resourceManager;

	if (aStatus == ANIM_TRIGGER)
	{
		float dist = sqrtf(((snail->x - px) * (snail->x - px)) + ((snail->y - py) * (snail->y - py)));
		if (dist < .75f)
			PlayerTakeDamage(rm->player, 34);
	}

	if (aStatus == ANIM_FINISHED)
		ChangeSnailState(snail, E_CHASE);
}

void ChangeSnailState(Entity* snail, ENEMY_STATE newState)
{

	snail->state = newState;
	
	switch (newState)
	{
	case E_NONE://err
		OutputDebugStringA("Enemy NONE!");
		break;
	case E_IDLE:
		ChangeAnimation(snail, snail->animator->animations[0]);
		break;
	case E_CHASE:
		ChangeAnimation(snail, snail->animator->animations[0]);
		break;
	case E_ATTACK:
		ChangeAnimation(snail, snail->animator->animations[1]);
		break;
	case E_PAIN:
		ChangeAnimation(snail, snail->animator->animations[2]);
		break;
	case E_DYING:
		ChangeAnimation(snail, snail->animator->animations[3]);
		break;
	case E_DEAD:
	{
		EnemyCustomData* data = (EnemyCustomData*) snail->CustomEntityData;
		data->dead = 1;//dead
		snail->texture = snail->animator->animations[3]->frames[4];//dead frame
		snail->solid = 0;
	}
		break;
	}
	//todo change things when swapping, maybe switch statement?
}


void SnailTakeDamage(Entity* snail, float damage)
{
	EnemyCustomData* d = (EnemyCustomData*)snail->CustomEntityData;
	d->hp -= damage;
	if (d->hp > 0)
	{
		if (snail->state != E_PAIN || snail->state != E_ATTACK)
			ChangeSnailState(snail, E_PAIN);
	}

	if(d->hp <= 0)
		ChangeSnailState(snail, E_DYING);
	//ResourceManager* rm = (ResourceManager*)resourceManager;
	//DeleteEntity(rm, snail);
}

int CheckEntity(Entity* snail, float radius)
{
	ResourceManager* rm = (ResourceManager*)resourceManager;

	for (int i = 0; i < rm->numEntities; i++)
	{
		if (snail == rm->entities[i] || rm->entities[i]->solid == 0)
			continue;

		float x2 = rm->entities[i]->x;
		float y2 = rm->entities[i]->y;

		float dist = sqrtf(((snail->x - x2) * (snail->x - x2)) + ((snail->y - y2) * (snail->y - y2)));
		
		if (dist <= radius)
			return 0;
	}

	return 1;
}

void CreateSnailAnimations(ResourceManager* rm)
{

	/// IDLE
	int snail = 5;
	Animation* animation = malloc(sizeof(Animation));
	animation->numFrames = 3;

	animation->frames = malloc(sizeof(Texture*) * animation->numFrames);
	animation->frames[0] = rm->textures[5];
	animation->frames[1] = rm->textures[6];
	animation->frames[2] = rm->textures[7];

	animation->frameLengths = malloc(sizeof(float*) * animation->numFrames);
	animation->frameLengths[0] = 0.6f;
	animation->frameLengths[1] = 0.6f;
	animation->frameLengths[2] = 0.6f;
	animation->numFrameTriggers = 0;
	animation->frameTriggers = NULL;
	//Set Duration
	animation->duration = 0;
	for (int i = 0; i < animation->numFrames; i++)
		animation->duration += animation->frameLengths[i];

	AddAnimation(rm, animation);

	/// ATTACK
	
	int snailAttack = 8;
	Animation* animation2 = malloc(sizeof(Animation));
	animation2->numFrames = 4;

	animation2->frames = malloc(sizeof(Texture*) * animation2->numFrames);
	animation2->frames[0] = rm->textures[snailAttack + 0];
	animation2->frames[1] = rm->textures[snailAttack + 1];
	animation2->frames[2] = rm->textures[snailAttack + 2];
	animation2->frames[3] = rm->textures[snailAttack + 3];

	animation2->frameLengths = malloc(sizeof(float*) * animation2->numFrames);
	animation2->frameLengths[0] = 0.2f;
	animation2->frameLengths[1] = 0.2f;
	animation2->frameLengths[2] = 0.5f;
	animation2->frameLengths[3] = 0.25f;
	
	animation2->numFrameTriggers = 1;
	animation2->frameTriggers = malloc(sizeof(int*));
	animation2->frameTriggers[0] = 2;

	//Set Duration
	animation2->duration = 0;
	for (int i = 0; i < animation2->numFrames; i++)
		animation2->duration += animation2->frameLengths[i];


	AddAnimation(rm, animation2);

	/// Pain

	int snailPain = 16;
	Animation* pain = malloc(sizeof(Animation));
	pain->numFrames = 3;

	pain->frames = malloc(sizeof(Texture*) * pain->numFrames);
	pain->frames[0] = rm->textures[snailPain + 0];
	pain->frames[1] = rm->textures[snailPain + 1];
	pain->frames[2] = rm->textures[snailPain + 2];

	pain->frameLengths = malloc(sizeof(float*) * pain->numFrames);
	pain->frameLengths[0] = 0.15f;
	pain->frameLengths[1] = 0.15f;
	pain->frameLengths[2] = 0.3f;

	pain->numFrameTriggers = 0;
	pain->frameTriggers = NULL;

	//Set Duration
	pain->duration = 0;
	for (int i = 0; i < pain->numFrames; i++)
		pain->duration += pain->frameLengths[i];

	AddAnimation(rm, pain);

	//DEATH

	int snailDeath = 19;
	Animation* death = malloc(sizeof(Animation));
	death->numFrames = 5;
	
	death->frames = malloc(sizeof(Texture*) * death->numFrames);
	death->frames[0] = rm->textures[snailDeath + 0];
	death->frames[1] = rm->textures[snailDeath + 1];
	death->frames[2] = rm->textures[snailDeath + 2];
	death->frames[3] = rm->textures[snailDeath + 3];
	death->frames[4] = rm->textures[snailDeath + 4];
	
	death->frameLengths = malloc(sizeof(float*) * death->numFrames);
	death->frameLengths[0] = 0.175f;
	death->frameLengths[1] = 0.175f;
	death->frameLengths[2] = 0.175f;
	death->frameLengths[3] = 0.25f;
	death->frameLengths[4] = 0.175f;
	
	death->numFrameTriggers = 0;
	death->frameTriggers = NULL;
	
	//Set Duration
	death->duration = 0;
	for (int i = 0; i < death->numFrames; i++)
		death->duration += death->frameLengths[i];
	
	AddAnimation(rm, death);
}
