	#include "Animation.h"

//TODO maybe return value to indicate animation status
/*
* 0) null
* 1) animEnd
* 2) trigger
*/

ANIM_STATUS UpdateAnimator(Animator* animator)
{
	Animation* anim = animator->currentAnimation;
	animator->currentFrameTime += (float)deltaTime;
	animator->currentAnimTime += (float)deltaTime;
	
	if (animator->currentFrameTime >= anim->frameLengths[animator->currentFrameIndex])
	{

		//Loop Animation, add optional don't loop? probably wont matter if i do return values
		if (animator->currentFrameIndex == anim->numFrames-1)
		{
			animator->currentAnimTime -= anim->duration;
			animator->currentFrameTime -= anim->frameLengths[animator->currentFrameIndex];
			animator->currentFrameIndex = 0;
			animator->currentFrame = animator->currentAnimation->frames[animator->currentFrameIndex];
			return ANIM_FINISHED;
		}
		else//if not looping
		{
			animator->currentFrameTime -= anim->frameLengths[animator->currentFrameIndex];
			animator->currentFrameIndex++;
			animator->currentFrame = animator->currentAnimation->frames[animator->currentFrameIndex];
			
		}
		
		for (int i = 0; i < anim->numFrameTriggers; i++)
			if (anim->frameTriggers[i] == animator->currentFrameIndex)
				return ANIM_TRIGGER;
	}

	return ANIM_NONE;
}

Texture* GetTexture(Animator* animator)
{
	return animator->currentFrame;
}