#pragma once	
#define _CRT_SECURE_NO_WARNINGS

#define CAPTURE_MOUSE 1

#include <time.h>
#include <windows.h>
#include <stdint.h>
#include "../game/Game.h"
#include "../Tools/ImageImporter.h"


void DrawPixel(int x, int y, int color);
int GetRGB(int r, int g, int b);
int GetKey( int keycode );
int GetMouse(int button);
int GetWidth();
int GetHeight();
double GetDeltaTime();

