#include "Draw.h"

void DrawSquare(int xx, int yy, int w, int h, int color)
{
	for (int y = yy; y < yy + h; y++)
		for (int x = xx; x < xx + w; x++)
		{
			DrawPixel(x, y, color);
		}
}

void DrawLine(int x1, int y1, int x2, int y2, int color)
{
	int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
	dx = x2 - x1;
	dy = y2 - y1;
	dx1 = abs(dx);
	dy1 = abs(dy);
	px = 2 * dy1 - dx1;
	py = 2 * dx1 - dy1;
	if (dy1 <= dx1)
	{
		if (dx >= 0)
		{
			x = x1;
			y = y1;
			xe = x2;
		}
		else
		{
			x = x2;
			y = y2;
			xe = x1;
		}
		DrawPixel(x, y, color);
		for (i = 0; x < xe; i++)
		{
			x = x + 1;
			if (px < 0)
				px = px + 2 * dy1;
			else
			{
				if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
					y = y + 1;
				else
					y = y - 1;
				px = px + 2 * (dy1 - dx1);
			}
			DrawPixel(x, y, color);

		}
	}
	else
	{
		if (dy >= 0)
		{
			x = x1;
			y = y1;
			ye = y2;
		}
		else
		{
			x = x2;
			y = y2;
			ye = y1;
		}
		DrawPixel(x, y, color);

		for (i = 0; y < ye; i++)
		{
			y = y + 1;
			if (py <= 0)
				py = py + 2 * dx1;
			else
			{
				if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
					x = x + 1;
				else
					x = x - 1;
				py = py + 2 * (dx1 - dy1);
			}
			DrawPixel(x, y, color);

		}
	}
}


//								IMAGE WIDTH + HEIGHT PRE SCALE
void DrawImage(int startX, int startY, int width, int height, int scaleX, int scaleY, int* image)
{
	int num = 0;
	for (int y = height - 1; y >= 0; y--)
		for (int x = 0; x < width; x++)
		{
			if (image[num] != 65535)
			{
				for (int sY = 0; sY < scaleY; sY++)
					for (int sX = 0; sX < scaleX; sX++)
					{
						DrawPixel(startX + (x * scaleX + sX), startY + (y * scaleY + sY), image[num]);
					}
			}
			num++;
		}
}

//probably a way to optimize this but whatever
void DrawLetter(int x, int y, int scaleX, int scaleY, int letter, int color)
{
					/// abcdefg
	char c = (char)letter;//0b0000000;
	//				A
	if((c >> (6)) & 1)
		DrawLine(x + 0 * scaleX, y + 0 * scaleY, x + 10 * scaleX, y + 0 * scaleY, color);

	//				F
	if ((c >> (5)) & 1)


		DrawLine(x + 0 * scaleX, y + 0 * scaleY, x + 0 * scaleX, y + 10 * scaleY, color);

	//				E
	if ((c >> (4)) & 1)


		DrawLine(x + 0 * scaleX, y + 10 * scaleY, x + 0 * scaleX, y + 20 * scaleY, color);

	//				D
	if ((c >> (3)) & 1)


		DrawLine(x + 0 * scaleX, y + 20 * scaleY, x + 10 * scaleX, y + 20 * scaleY, color);

	//				B
	if ((c >> (2)) & 1)


		DrawLine(x + 10 * scaleX, y + 0 * scaleY, x + 10 * scaleX, y + 10 * scaleY, color);

	//				C
	if ((c >> (1)) & 1)


		DrawLine(x + 10 * scaleX, y + 10 * scaleY, x + 10 * scaleX, y + 20 * scaleY, color);

	//				G
	if ((c >> (0)) & 1)
		DrawLine(x + 0 * scaleX, y + 10 * scaleY, x + 10 * scaleX, y + 10 * scaleY, color);

}

//ONLY SUPPORTS 3 NUMBERS
//FIX LEADING 0s
void DrawMultiNumber(int x, int y, int sX, int sY, int num, int color)
{
	//CAPS OUT AT 3 DIGITS
	int numDigits = 0;
	int n = num;

	//get amount of digits in number
	while (n!= 0)
	{
		int digit = n % 10;
		n= n/ 10;
		//PrintInt("%d\n", digit);
		
		numDigits++;
	}

	for (int i = 0; i < 3; i++)
	{


		char a = num % 10;
		num /= 10;
		a += '0';

		int drawID = -1;

		switch (a)
		{
		case '0':
			drawID = L_0;
			break;
		case '1':
			drawID = L_1;
			break;
		case '2':
			drawID = L_2;
			break;
		case '3':
			drawID = L_3;
			break;
		case '4':
			drawID = L_4;
			break;
		case '5':
			drawID = L_5;
			break;
		case '6':
			drawID = L_6;
			break;
		case '7':
			drawID = L_7;
			break;
		case '8':
			drawID = L_8;
			break;
		case '9':
			drawID = L_9;
			break;
		default:
			drawID = 5457;
			break;

		}
		DrawLetter(x + ((numDigits-i) * 12 * sX), y, 1, 1, drawID, color);
	}


	//OutputDebugStringA("\n\n\n\n");

}