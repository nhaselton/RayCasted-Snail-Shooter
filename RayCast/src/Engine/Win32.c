#include "Win32.h"
#include "global.h"
#include <windowsx.h>
#define PRINT_FPS 0

//easier to change here than wimain
const int SetWidth = 640;
const int SetHeight = 640;

int running = 1;
BITMAPINFO bitmapInfo;
void* bitmapMemory;
int pixelSize = 4;
int midX = 0;
//char keysDown[256];
//double deltaTime;


int GetMouse( int button ) {
	//error check
	if ( button > 10 )
		return 0;
	//return value
	return mouseDown[button];
}

int GetRGB(int r, int g, int b)
{
	return b | g << 8 | r << 16;
}

void DrawPixel(int x, int y, int color)
{
	int location = ((bitmapHeight - 1 - y) * bitmapWidth+ x);
	
	//if oob 
	if (x >= bitmapWidth || y>= bitmapHeight || x <= 0 || y <= 0)
		return;
	
	int* buffer = (int*)bitmapMemory;
	buffer[location] = (color);
}

int GetKey(int keycode)
{
	if (keycode < 256)
	{
		return keysDown[keycode];
	}
}

int GetWidth()
{
	return bitmapWidth;
}

int GetHeight()
{
	return bitmapHeight;
}

void Win32ResizeDIBSection(int width, int height)
{
	if (bitmapMemory)
		VirtualFree(bitmapMemory, 0, MEM_RELEASE);

	bitmapWidth = width;
	bitmapHeight = height;

	bitmapInfo.bmiHeader.biSize = sizeof(bitmapInfo.bmiHeader);
	bitmapInfo.bmiHeader.biWidth = bitmapWidth;
	bitmapInfo.bmiHeader.biHeight = bitmapHeight ;
	bitmapInfo.bmiHeader.biPlanes = 1;
	bitmapInfo.bmiHeader.biBitCount = 32;
	bitmapInfo.bmiHeader.biCompression = BI_RGB;
	int bitmapMemorySize = (bitmapWidth * bitmapHeight) * pixelSize;//4 bytes to allign

	bitmapMemory = VirtualAlloc(0, bitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);


}

void Win32UpdateWindow(HDC deviceContext, RECT* windowRect, int x, int y, int w, int h)
{
	//StretchDIBits(deviceContext, x, y, w, h, x, y, w, h, bitmapMemory, &bitmapInfo, DIB_RGB_COLORS, SRCCOPY);
	int windowWidth = windowRect->right - windowRect->left;
	int windowHeight = windowRect->bottom - windowRect->top;
	int* bm = (int*)bitmapMemory;
	StretchDIBits(deviceContext, 0, 0, bitmapWidth, bitmapHeight, 0, 0, windowWidth, windowHeight, bitmapMemory, &bitmapInfo, DIB_RGB_COLORS, SRCCOPY);
}


LRESULT CALLBACK
Win32MainWindowCallback(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;
	switch (message)
	{
	case WM_SIZE: //change size
	{
		RECT clientRect;
		GetClientRect(window, &clientRect);

		int w = clientRect.right - clientRect.left;
		int h  = clientRect.bottom - clientRect.top;
		Win32ResizeDIBSection(w, h);
		OutputDebugString(L"WM_SIZE\n");

	}
	break;
	case WM_DESTROY://destorys window
	{
		OutputDebugString(L"WM_DESTROY\n");
		running = 0;
	}
	break;
	case WM_CLOSE://close button
	{
		running = 0;
		OutputDebugString(L"WM_CLOSE\n");
		exit( 0 );
	}
	break;
	case WM_ACTIVATEAPP://startup
	{
		OutputDebugString(L"WM_ACTIVATEAPP\n");
	}
	break;

	case WM_SYSKEYDOWN:
	{
		int keyCode = wParam;
	}
	break;
	case WM_SYSKEYUP:
	{
		int keyCode = wParam;
	}
	break;

	case WM_KEYDOWN:
	{
		int keyCode = wParam;
		keysDown[keyCode] = 1;
		if (keyCode == VK_ESCAPE)
			exit(0);
	}
	break;
	case WM_KEYUP:
	{
		int keyCode = wParam;
		keysDown[keyCode] = 0;
	}
	break;

	case WM_PAINT:
	{
		PAINTSTRUCT paint;
		HDC deviceContext = BeginPaint(window, &paint);
		int x = paint.rcPaint.left;
		int y = paint.rcPaint.top;
		int w = paint.rcPaint.right - paint.rcPaint.left;
		int h = paint.rcPaint.bottom - paint.rcPaint.top;

		RECT clientRect;
		GetClientRect(window, &clientRect);

		Win32UpdateWindow(deviceContext, &clientRect, x, y, w, h);
		EndPaint(window, &paint);
	}

	case WM_MOUSEMOVE:
	{
		WINDOWINFO w;
		w.cbSize = sizeof( WINDOWINFO );
		GetWindowInfo( window, &w );

		int x = ( int ) GET_X_LPARAM( lParam );
		int test = w.rcWindow.left + 8;
		
		currentMouse = x + test - midX;
		currentMouse *= 2500 * ( float ) deltaTime;
		break;
	}
	case WM_LBUTTONDOWN:
	{
		mouseDown[0] = 1;
	}break;

	case WM_LBUTTONUP:
	{
		mouseDown[0] = 0;
	}break;



	default:
		result = DefWindowProc(window, message, wParam, lParam);
		break;
	}
	return result;
}
double GetDeltaTime()
{
	return deltaTime;
}

int CALLBACK
WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdShow)
{
	//set width&height
	screenWidth = SetWidth;
	screenHeight = SetHeight;

	//set all keys to down
	for ( int i = 0; i < 256; i++ )keysDown[i] = 0;
	for (int i = 0; i < 10; i++)mouseDown[i] = 0;
	
	//set up mouse input
	currentMouse = 0;

	WNDCLASS windowClass = { 0 };
	windowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = Win32MainWindowCallback;
	windowClass.hInstance = instance;
	windowClass.lpszClassName = L"Window";

	RegisterClass(&windowClass);

	//Creates Window
	HWND windowHandle = CreateWindowEx(0, windowClass.lpszClassName, L"Game (i hope)", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, screenWidth, screenHeight,//x,y,w,h
		0, 0, instance, 0);


	//if worked
	long last; 
	GetSystemTimeAsFileTime((FILETIME*)&last);


	
	if (windowHandle)
	{

		WINDOWINFO w;
		w.cbSize = sizeof( WINDOWINFO );
		GetWindowInfo( windowHandle, &w );

		midX = w.rcWindow.left + ( w.rcWindow.right / 2 );
		int midY = w.rcWindow.top + ( w.rcWindow.bottom / 2 );


		InitGame();
		while (running)
		{
			long now; GetSystemTimeAsFileTime((FILETIME*)&now);
			currentTime = now; //Setting Global (nothing to do with timing here)
			long dt = (now-last);
			last = now;
			deltaTime = ((double)dt)/ 10000000;
#if PRINT_FPS
			PrintDouble("Fps: %lf\n",1.0 / deltaTime);
#endif



			MSG message;
			while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
			{
				if (message.message == WM_QUIT)
					running = 0;

				TranslateMessage(&message);
				DispatchMessageA(&message);
			}
		
			UpdateGame();
			currentMouse = 0;

			//Draw New Buffer
			HDC deviceContext = GetDC(windowHandle);
			RECT windowRect;
			GetClientRect(windowHandle, &windowRect);
			Win32UpdateWindow(deviceContext, &windowRect, 0, 0, bitmapWidth, bitmapHeight);
//#ifdef CAPTURE_MOUSE
			
			//get window in screen space
			

			midX = w.rcWindow.left + ( w.rcWindow.right / 2 );
			int midY = w.rcWindow.top + ( w.rcWindow.bottom / 2 );
			
			SetCursorPos( midX, midY );
			ShowCursor( 1 );
		}

	}
	return 0;
}