#pragma once
#include "../Tools/ImageImporter.h"
#include <Windows.h>
typedef struct Map
{
	int width;
	int height;
	unsigned char* layout;
} Map;

CheckFree(Map* map, float x, float y);