#pragma once
#include "../Tools/ImageImporter.h"
#include "../Game/Player.h"
#include "../Game/Entity.h"
#include "../game/map.h"
#include "../Tools/Helper.h"
#include "Render/Animation/Animation.h"
#include "../Game/Weapons/Weapon.h"

typedef struct ResourceManager
{
	Texture** textures;
	int numTextures;
	int capTextures;

	Entity** entities;
	int numEntities;
	int capEntities;

	Animation** animations;
	int numAnimations;
	int capAnimations;

	Player* player;
	Map* currentMap;

	Weapon** weapons;
	int numWeapons;
	int capWeapons;

}ResourceManager;

ResourceManager CreateResourceManager();
void AddTexture(ResourceManager* rm, const char* path);
Map CreateMap(const char* path,ResourceManager* rm);
Entity* AddEntity(ResourceManager* rm, float x, float y, Texture* texture);
Entity* AddEntityCustomSize(ResourceManager* rm, int x, int y, Texture* texture, int udiv, int vdiv, int vmove);
Animation* AddAnimation(ResourceManager* rm, Animation* anim);
Weapon* AddWeapon(ResourceManager* rm);
void DeleteEntity(ResourceManager* rm,Entity* entity);
