#include <windows.h>
#include <stdint.h>

#include "game.h"


static bool running;
static int bitmap_width, bitmap_height;
static BITMAPINFO bitmap_info;
static void* bitmap_memory;


static void render_background(int red, int green, int blue)
{
	int bytes_per_pixel = 4;
	int pitch = bitmap_width * bytes_per_pixel;
	uint8_t* row = (uint8_t*)bitmap_memory;
	for (int y = 0; y < bitmap_height; y++)
	{
		uint8_t* pixel = (uint8_t*)row;
		for (int x = 0; x < bitmap_width; x++)
		{
			// NOTE : little endian architecture
			//		  pixel in memory : BB GG RR xx
			//					  blue  green  red  pad byte
			*pixel = blue;
			++pixel;

			*pixel = green;
			++pixel;

			*pixel = red;
			++pixel;

			*pixel = 0;
			++pixel;
		}
		row += pitch;
	}
}

// resizes the Device Independent Buffer (DIB) section
static void resizeFrameBuffer(int width, int height)
{
	if (bitmap_memory)
	{
		VirtualFree(bitmap_memory, 0, MEM_RELEASE);
	}

	bitmap_width = width;
	bitmap_height = height;
	
	bitmap_info.bmiHeader.biSize = sizeof(bitmap_info.bmiHeader);
	bitmap_info.bmiHeader.biWidth = bitmap_width;
	bitmap_info.bmiHeader.biHeight = -bitmap_height;
	bitmap_info.bmiHeader.biPlanes = 1;
	bitmap_info.bmiHeader.biBitCount = 32;
	bitmap_info.bmiHeader.biCompression = BI_RGB;

	int bytes_per_pixel = 4;
	int bitmap_memory_size = bitmap_width * bitmap_height * bytes_per_pixel;
	bitmap_memory = VirtualAlloc(0, bitmap_memory_size, MEM_COMMIT, PAGE_READWRITE);

	render_background(250, 180, 200);
}

static void updateWindow(HDC device_context, RECT* window_rect)
{
	int window_width = window_rect->right - window_rect->left;
	int window_height = window_rect->bottom - window_rect->top;
	StretchDIBits(
		device_context,
		0, 0, bitmap_width, bitmap_height,
		0, 0, bitmap_width, bitmap_height,
		bitmap_memory,
		&bitmap_info,
		DIB_RGB_COLORS,
		SRCCOPY
	);
}

LRESULT CALLBACK PrimaryWindowCallback(
	HWND window_handle,
	UINT message,
	WPARAM wParam,
	LPARAM lParam
)
{
	LRESULT result = 0;
	
	switch (message)
	{
		case WM_SIZE:
		{
			RECT client_rect;
			GetClientRect(window_handle, &client_rect);
			resizeFrameBuffer(
				client_rect.right - client_rect.left,
				client_rect.bottom - client_rect.top
			);

			OutputDebugStringA("Window resize\n");
		} break;

		case WM_CLOSE:
		{
			running = false;
			OutputDebugStringA("Window close\n");
		} break;

		case WM_DESTROY:
		{
			running = false;
			OutputDebugStringA("Window destroy\n");
		} break;

		case WM_ACTIVATEAPP:
		{
			OutputDebugStringA("Window toggle focus\n");
		} break;

		case WM_PAINT:
		{	
			PAINTSTRUCT paint;
			HDC device_context = BeginPaint(window_handle, &paint);

			int x = paint.rcPaint.left;
			int	y = paint.rcPaint.top;
			int width = paint.rcPaint.right - paint.rcPaint.left;
			int height = paint.rcPaint.bottom - paint.rcPaint.top;

			RECT client_rect;
			GetClientRect(window_handle, &client_rect);
			updateWindow(device_context, &client_rect);
			
			EndPaint(window_handle, &paint);
		} break;

		default:
		{
			result = DefWindowProc(window_handle, message, wParam, lParam);
		} break;
	}

	return result;
}

INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow) // NOTE: MSDN
{
	int screen_x, screen_y, screen_width, screen_height;
	init_game(&screen_width, &screen_height);

	// TODO: calculate x and y for a window to be in center
	// for now, it's hardcoded
	screen_x = 250;
	screen_y = 250;
	
	WNDCLASSA window_class = {};	// init every member to 0
	window_class.style = CS_CLASSDC | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	window_class.lpfnWndProc = PrimaryWindowCallback;
	window_class.hInstance = hInstance;
	//window_class.hIcon;
	window_class.lpszClassName = "RiseGame_WindowClass";

	if (!RegisterClass(&window_class))
	{
		OutputDebugStringA("Failed to create window (class did not register)\n");
		return -1;	
	}

	// TODO : deal with the fact that x, y, width and height arguments of the CreateWindowEx is
	//		  taking the section with title and min/max/close buttons into account
	HWND window_handle = CreateWindowExA(
		0,
		window_class.lpszClassName,
		"Rise Game",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		screen_x,
		screen_y,
		screen_width,
		screen_height,
		0,
		0,
		hInstance,
		0
	);

	if (window_handle)
	{
		running = true;
		
		while (running)
		{
			MSG message;
			if (GetMessage(&message, 0, 0, 0))
			{
				TranslateMessage(&message);
				DispatchMessage(&message);		// Send message to the WindowProc
			}
			else
			{
				break;
			}
		}
	}
	
	return 0;
}