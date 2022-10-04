#include "ResourceManager.h"

ResourceManager CreateResourceManager()
{
	ResourceManager rm;
	rm.capTextures = 10;
	rm.numTextures = 0;
	rm.textures = (int**)malloc(rm.capTextures * sizeof(int*));
	rm.numEntities = 0;
	rm.capEntities = 5;
	rm.entities = (Entity**)malloc(rm.capEntities * sizeof(Entity*));
	
	rm.numAnimations = 0;
	rm.capAnimations = 10;
	rm.animations = (Animation**)malloc(rm.capAnimations * sizeof(Animation*));
		
	rm.numWeapons = 0;
	rm.capWeapons = 5;
	rm.weapons = (Weapon**) malloc(rm.capWeapons* sizeof(Weapon*));
	return rm;
}

void AddTexture(ResourceManager* rm, const char* path)
{
	Texture* texture = ReadBMP(path);

	//Make sure of room
	if (rm->numTextures == rm->capTextures)
	{
		rm->capTextures *= 2;
		rm->textures = realloc(rm->textures, rm->capTextures * sizeof(int*));
	}
	//add new textuer
	rm->textures[rm->numTextures] = texture;
	rm->numTextures++;
}

Map CreateMap(const char* path,ResourceManager* rm)
{
	Map map;
	FILE* file = fopen(path, "r");
	if (!file)
		OutputDebugStringA("COULDNT OPEN MAP FILE");
	
	int width = -1;
	int height = -1;
	int num = 0;
	int newLines = 0;
	//max map size
	char mapBuffer[1024];
	memset(mapBuffer, '.', 1024);

	while (1)
	{
		char c = fgetc(file);

		if (c == EOF)
			break;

		switch (c)
		{
		case '\n':
			if (width == -1)
				width = num;
			newLines++;
			break;
		case'b':
		{
			//if first line go at num else mod (modding by 0 is UD)
			int eX = (newLines == 0) ? num : (num)%width;
			AddEntity(rm, eX, newLines, rm->textures[3]);
			num++;
		}
			break;
		case'.':
			num++;
			break;
		default:
			mapBuffer[num] = c;
			num++;
			break;
		}
	}
	//set params
	height = num/width;
	map.width = width;
	map.height = height;
	//take data from buffer and add to layout
	map.layout = (char*)malloc(width * height);
	memcpy(map.layout, mapBuffer, map.width * map.height);
	fclose(file);
	rm->currentMap = &map;
	return map;
}

Entity* AddEntity(ResourceManager* rm, float x, float y, Texture* texture)
{
	//VECTOR FUNCTIONALITY
	Entity* entity = (Entity*) malloc(sizeof(Entity));
	//returns pointer to new entity incase needed
	if (rm->numEntities == rm->capEntities)
	{
		rm->capEntities *= 2;
		rm->entities = realloc(rm->entities, rm->capEntities * sizeof(Entity*));
	}
	rm->entities[rm->numEntities] = entity;
	rm->numEntities++;

	//== ENTITY CREATION == \\ 
	entity->texture = texture;
	entity->visible = texture != NULL;
	entity->x = x;
	entity->y = y;
	entity->uDiv = 1;
	entity->vDiv = 1;
	entity->vMove = 0;
	entity->width = texture->width;
	entity->height = texture->height;
	//scuffed function pointer go brrr
	entity->animator = NULL;
	entity->state = E_NONE;

	entity->update = (UpdateStatic);
	entity->changeState = (ChangeStateStatic);
	entity->PlayerTakeDamage = NULL;
	entity->solid = 1;

	return entity;
}

Entity* AddEntityCustomSize(ResourceManager* rm,int x,int y,Texture* texture, int udiv,int vdiv,int vmove)
{
	Entity* entity = AddEntity(rm, x, y, texture);
	entity->vDiv = vdiv;
	entity->uDiv = udiv;
	entity->vMove = vmove;
	return entity;
}

//TODO make in files?
Animation* AddAnimation(ResourceManager* rm, Animation* anim)
{
	//TODO FIX 
	rm->animations[rm->numAnimations] = anim;
	rm->numAnimations++;
}

Weapon* AddWeapon(ResourceManager* rm)
{
	Weapon* weapon = malloc(sizeof(Weapon));
	weapon->extraInfo = NULL;
	weapon->animator = NULL;
	weapon->Shoot = Shoot;
	weapon->cooldown = 0;
	weapon->maxCooldown = 0;
	weapon->state = WEAPON_IDLE;
	rm->weapons[0] = weapon;
	return weapon;

}

void DeleteEntity(ResourceManager* rm, Entity* entity)
{
	int locationInArray = -1;
	
	//find entity location
	for (int i = 0; i < rm->numEntities; i++)
		if (rm->entities[i] == entity)
		{
			locationInArray = i;
			break;
		}

	//Replace with death instead of free?
	free(entity);
		
	for (int i = locationInArray; i < rm->numEntities; i++)
	{
		rm->entities[i] = rm->entities[i + 1];
	}
	
	rm->numEntities-=1;
}