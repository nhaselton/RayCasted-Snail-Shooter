#pragma once
#include "../Entity.h"

typedef enum EnemyState
{
	E_NONE		= 0,
	E_CHASE		= 1,
	E_ATTACK	= 2,
	E_PAIN		= 3,
	E_DYING		= 4,
	E_DEAD		= 5,
	E_IDLE		= 6

}ENEMY_STATE;


typedef struct EnemyCustomData
{
	float hp;
	int dead;
}EnemyCustomData;