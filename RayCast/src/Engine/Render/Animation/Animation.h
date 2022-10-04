#pragma once
#include "../Texture.h"
#include "../../../Engine/Global.h"

typedef enum ANIM_STATUS
{
	ANIM_NONE = 0,
	ANIM_FINISHED = 1,
	ANIM_TRIGGER = 2
} ANIM_STATUS;

typedef struct Animation
{
	Texture** frames;
	int numFrames;
	float* frameLengths;
	float duration;

	int* frameTriggers;
	int numFrameTriggers;
}Animation;

typedef struct Animator
{
	Animation** animations;
	Animation* currentAnimation;
	float currentAnimTime;
	float currentFrameTime;
	int currentFrameIndex;
	Texture* currentFrame;
	
}Animator;


ANIM_STATUS UpdateAnimator(Animator* animator);
Texture* GetTexture(Animator* animator);