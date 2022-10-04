#include "Player.h"

void GetInput(Player* player, Map* map)
{
	if (GetKey(VK_W))
	{
		float xOffset = (player->dirX * playerSpeed * (float)deltaTime);
		float yOffset = (player->dirY * playerSpeed * (float)deltaTime);

		int pX = (int)(player->x + xOffset);
		int pY = (int)(player->y + yOffset);

		//if (map->layout[pY * 12 + (int)player->x] != '.')
		if (CheckFree(map, player->x, (float)pY) == 1)
		{
			player->y += player->dirY * playerSpeed * (float)deltaTime;

		}
		//if (map->layout[(int)player->y * 12 + pX] != '.')
		if (CheckFree(map, (float)pX, player->y) == 1)
		{
			player->x += player->dirX * playerSpeed * (float)deltaTime;
		}
	}
	if (GetKey(VK_S))
	{
		float xOffset = (player->dirX * playerSpeed * (float)deltaTime);
		float yOffset = (player->dirY * playerSpeed * (float)deltaTime);

		int pX = (int)(player->x - xOffset);
		int pY = (int)(player->y - yOffset);

		//if (map->layout[pY * 12 + (int)player->x] != '.')
		if (CheckFree(map, player->x, (float)pY) == 1)
		{
			player->y -= player->dirY * playerSpeed * (float)deltaTime;

		}
		//if (map->layout[(int)player->y * 12 + pX] != '.')

		if (CheckFree(map, (float)pX, player->y) == 1)
		{
			player->x -= player->dirX * playerSpeed * (float)deltaTime;
		}
	}
	if (GetKey(VK_A))
	{
		float xOffset = (player->planeX * playerSpeed * (float)deltaTime);
		float yOffset = (player->planeY * playerSpeed * (float)deltaTime);

		int pX = (int)(player->x - xOffset);
		int pY = (int)(player->y - yOffset);

		//if (map->layout[pY * 12 + (int)player->x] != '.')
		if (CheckFree(map, player->x, (float)pY) == 1)

		{
			player->y -= yOffset;

		}
		//if (map->layout[(int)player->y * 12 + pX] != '.')
		if (CheckFree(map, (float)pX, player->y) == 1)
		{
			player->x -= xOffset;
		}

	}
	if (GetKey(VK_D))
	{
		float xOffset = (player->planeX * playerSpeed * (float)deltaTime);
		float yOffset = (player->planeY * playerSpeed * (float)deltaTime);

		int pX = (int)(player->x + xOffset);
		int pY = (int)(player->y + yOffset);

		//if (map->layout[pY * 12 + (int)player->x] != '.')
		if (CheckFree(map, player->x, (float)pY) == 1)

		{
			player->y += yOffset;

		}
		//if (map->layout[(int)player->y * 12 + pX] != '.')
		if (CheckFree(map, (float)pX, player->y) == 1)
		{
			player->x += xOffset;
		}
	}

	// == TURNING ==\\	 
	float currentTurnspeed = 0;
	
	//if turn left or right with keys, edit the turn speed
	if ( GetKey( VK_LEFT ) )
		currentTurnspeed += playerTurnSpeed;
	if (GetKey(VK_RIGHT) )
		currentTurnspeed -= playerTurnSpeed;
	
	//check mouse delta from center of the screen
	currentTurnspeed -=  currentMouse / 10.0f;


	if ( currentTurnspeed != 0 ) {
		float oldDirX = player->dirX;
		player->dirX = player->dirX * cosf( -currentTurnspeed * deltaTime ) - player->dirY * sinf( -currentTurnspeed * deltaTime );
		player->dirY = oldDirX * sinf( -currentTurnspeed * deltaTime ) + player->dirY * cosf( -currentTurnspeed * deltaTime );
		double oldPlaneX = player->planeX;

		player->planeX = player->planeX * cosf( -currentTurnspeed * deltaTime ) - player->planeY * sinf( -currentTurnspeed * deltaTime );
		player->planeY = oldPlaneX * sinf( -currentTurnspeed * deltaTime ) + player->planeY * cosf( -currentTurnspeed * deltaTime );
	}


	if (GetKey(VK_CONTROL) || GetMouse(0))
	{
		Weapon* weapon = player->currentWeapon;
		if (weapon->cooldown <= 0 && weapon->state == WEAPON_IDLE)
		{
			weapon->state = WEAPON_SHOOTING;
			//void(*u)() = weapon->Shoot;
			//u(player,map);
			Weapon* weapon = player->currentWeapon;
			ChangePlayerWeaponAnimation(weapon->animator, weapon->animator->animations[1]);
			player->ammo--;

		}
	}


}

void ChangePlayerWeaponAnimation(Animator* animator, Animation* animation)
{
	animator->currentAnimation = animation;
	animator->currentFrameIndex = 0;
	animator->currentFrame = animator->currentAnimation->frames[0];
	animator->currentFrameTime = 0;
	animator->currentAnimTime = 0;

}

void UpdatePlayer(Player* player,Map* map)
{
	//UPDATES WEAPON
	Weapon* weapon = player->currentWeapon;
	ANIM_STATUS status = UpdateAnimator(weapon->animator);
	WEAPON_STATE wState = weapon->state;
	switch (wState)
	{
	case WEAPON_IDLE:
		break;
	case WEAPON_COOLDOWN:
		break;
	case WEAPON_SHOOTING:
		if (status == ANIM_FINISHED)
		{
			ChangePlayerWeaponAnimation(weapon->animator, weapon->animator->animations[0]);
			weapon->state = WEAPON_IDLE;
		}
		else if (status == ANIM_TRIGGER)
		{
			void(*u)() = weapon->Shoot;
			u(player,map);

		}
		break;

	}
}

void PlayerTakeDamage(Player* player, float damage)
{
	player->hp -= damage;
}