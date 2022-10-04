#include "map.h"

CheckFree(Map* map, float x, float y)
{
	char c = map->layout[(int) ((int)y * map->width + (int)x)];
	return c == '.';
	//switch (c)
	//{
	////case ' ':
	////	return 1;
	////	break;
	////case '.':
	////	return 0;
	////	break;
	////default:
	////	return 0;
	////	break;
	//	case '.'
	//		return 1;
	//		de
	//}
}