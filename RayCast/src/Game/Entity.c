#include "Entity.h"

void UpdateStatic(Entity* entity, float px,float py)
{
}

void ChangeStateStatic(Entity* entity,ENEMY_STATE state)
{

}

void ChangeAnimation(Entity* entity, Animation* animation)
{
	entity->animator->currentAnimation = animation;
	entity->animator->currentFrameIndex = 0;
	entity->animator->currentFrame = entity->animator->currentAnimation->frames[0];
	entity->animator->currentFrameTime = 0;
	entity->animator->currentAnimTime = 0;
}