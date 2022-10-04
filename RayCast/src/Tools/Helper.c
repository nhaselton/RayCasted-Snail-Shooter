#include "Helper.h"
void PrintFloat(char* string, float var)
{
	char buffer[250];
	sprintf(buffer, string, var);
	OutputDebugStringA(buffer);
}

void PrintInt(char* string, int var)
{
	char buffer[250];
	sprintf(buffer, string, var);
	OutputDebugStringA(buffer);
}

void PrintDouble(char* string, double var)
{
	char buffer[250];
	sprintf(buffer, string, var);
	OutputDebugStringA(buffer);
}