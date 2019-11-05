#include <windows.h>
#include <stdint.h>
#include <xinput.h>

#include "game.h"


struct BitmapBuffer
{
	int width, height;
	BITMAPINFO info;
	void* memory;
	int pitch;	// in bytes
	int bytes_per_pixel = 4;
};

struct WindowDimensions
{
	int width, height;
};


static bool running;
static BitmapBuffer backbuffer;


//-----------------------------------------------------------------------------------------------------------------
// solving the unresolved externals linking problem when using XInputGetState and XInputSetState (loading the dll)
typedef DWORD WINAPI xinput_get_state(DWORD dwUserIndex, XINPUT_STATE* pState);
typedef DWORD WINAPI xinput_set_state(DWORD dwUserIndex, XINPUT_VIBRATION* pVibration);

// NOTE : stub functions (functions with no implementation), they will be used if xinput library failed to load
// because we want to be able to play the game even if we don't have a library to support gamepad input (use keyboard instead)
DWORD WINAPI xinput_get_state_stub(DWORD dwUserIndex, XINPUT_STATE* pState)
{
	return ERROR_DEVICE_NOT_CONNECTED;
}
DWORD WINAPI xinput_set_state_stub(DWORD dwUserIndex, XINPUT_VIBRATION* pVibration)
{
	return ERROR_DEVICE_NOT_CONNECTED;
}

static xinput_get_state* XInputGetState_ = xinput_get_state_stub;
static xinput_set_state* XInputSetState_ = xinput_set_state_stub;
#define XInputGetState XInputGetState_
#define XInputSetState XInputSetState_

static void LoadXInput()
{
	HMODULE XInputLibrary = LoadLibrary("xinput1_4.dll");
	// if version 1.4 of xinput is not supported by the os, load version 1.3
	// NOTE : made primarily to support windows 8, since windows 8 has only one version of xinput which is 1.4
	if (!XInputLibrary)
	{
		HMODULE XInputLibrary = LoadLibrary("xinput1_3.dll");
	}

	if (XInputLibrary)
	{
		XInputGetState = (xinput_get_state*) GetProcAddress(XInputLibrary, "XInputGetState");
		XInputSetState = (xinput_set_state*) GetProcAddress(XInputLibrary, "XInputSetState");
	}
}
//-----------------------------------------------------------------------------------------------------------------

WindowDimensions getWindowDimensions(HWND window_handle)
{
	RECT client_rect;
	GetClientRect(window_handle, &client_rect);

	return { client_rect.right - client_rect.left, client_rect.bottom - client_rect.top };
}

static void render_background(BitmapBuffer* buffer, int red, int green, int blue)
{
	int pitch = buffer->width * buffer->bytes_per_pixel;
	uint8_t* row = (uint8_t*)buffer->memory;
	for (int y = 0; y < buffer->height; y++)
	{
		uint8_t* pixel = (uint8_t*)row;
		for (int x = 0; x < buffer->width; x++)
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
static void resizeFrameBuffer(BitmapBuffer* buffer, int width, int height)
{
	if (buffer->memory)
	{
		VirtualFree(buffer->memory, 0, MEM_RELEASE);
	}

	buffer->width = width;
	buffer->height = height;
	
	buffer->info.bmiHeader.biSize = sizeof(buffer->info.bmiHeader);
	buffer->info.bmiHeader.biWidth = buffer->width;
	buffer->info.bmiHeader.biHeight = -(buffer->height);
	buffer->info.bmiHeader.biPlanes = 1;
	buffer->info.bmiHeader.biBitCount = 32;
	buffer->info.bmiHeader.biCompression = BI_RGB;

	int bitmap_memory_size = buffer->width * buffer->height * buffer->bytes_per_pixel;
	buffer->memory = VirtualAlloc(0, bitmap_memory_size, MEM_COMMIT, PAGE_READWRITE);

	buffer->pitch = buffer->width * buffer->bytes_per_pixel;

	render_background(buffer, 250, 230, 150);
}

static void copyBufferToWindow(BitmapBuffer* buffer, HDC device_context, int window_width, int window_height)
{
	StretchDIBits(
		device_context,
		0, 0, window_width, window_height,
		0, 0, buffer->width, buffer->height,
		buffer->memory,
		&(buffer->info),
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

		// keyboard input handling
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
		case WM_KEYDOWN:
		case WM_KEYUP:
		{
			uint32_t VKCode = wParam;

			bool was_down	= (lParam & (1 << 30)) != 0;
			bool is_down	= (lParam & (1 << 31)) == 0;
			if (was_down != is_down)
			{
				if (VKCode == 'W')
				{
					if (was_down && !is_down)
						OutputDebugStringA("W is released!\n");
				}
				else if (VKCode == 'A')
				{

				}
				else if (VKCode == 'S')
				{

				}
				else if (VKCode == 'D')
				{

				}
				else if (VKCode == 'Q')
				{

				}
				else if (VKCode == 'E')
				{

				}
				else if (VKCode == VK_UP)
				{

				}
				else if (VKCode == VK_DOWN)
				{

				}
				else if (VKCode == VK_LEFT)
				{

				}
				else if (VKCode == VK_RIGHT)
				{

				}
				else if (VKCode == VK_SPACE)
				{

				}
				else if (VKCode == VK_ESCAPE)
				{

				}

				// handle the close operation with alt+F4
				bool alt_key_was_down = (lParam & (1 << 29)) != 0;
				if ((VKCode == VK_F4) && alt_key_was_down)
				{
					running = false;
					// TODO : handle the game destroy
				}
			}
		} break;

		/*case WM_SETCURSOR:
		{
			SetCursor(0);
		} break;*/

		case WM_PAINT:
		{	
			PAINTSTRUCT paint;
			HDC device_context = BeginPaint(window_handle, &paint);

			WindowDimensions window_dimensions = getWindowDimensions(window_handle);
			copyBufferToWindow(&backbuffer, device_context, window_dimensions.width, window_dimensions.height);
			
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
	// load the XInput dll library
	LoadXInput();
	
	WNDCLASSA window_class = {};	// init every member to 0

	int screen_x;
	int screen_y;
	int screen_width;
	int screen_height;

	get_user_resolution(&screen_width, &screen_height);

	// TODO: calculate x and y for a window to be in center
	// for now, it's hardcoded
	screen_x = 500;
	screen_y = 500;
	
	resizeFrameBuffer(&backbuffer, screen_width, screen_height);

	window_class.style = CS_HREDRAW | CS_VREDRAW;
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
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
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
			// process windows messages
			MSG message;
			while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
			{
				if (message.message == WM_QUIT)
					running = false;
					// TODO : handle the game destroy (call a function)
				
				TranslateMessage(&message);
				DispatchMessage(&message);		// Send message to the WindowProc
			}

			// get gamepad input
			for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
			{
				XINPUT_STATE controller_state;
				//ZeroMemory(&state, sizeof(XINPUT_STATE));

				// Get the state of the controller from XInput
				if (XInputGetState(i, &controller_state) == ERROR_SUCCESS)	// Controller is connected
				{ 
					bool up				= (controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP);
					bool down			= (controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
					bool left			= (controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
					bool right			= (controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
					bool start			= (controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_START);
					bool back			= (controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK);
					bool left_thumb		= (controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB);
					bool right_thumb	= (controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB);
					bool left_shoulder	= (controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER);
					bool right_shoulder = (controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER);
					bool a_button		= (controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_A);
					bool b_button		= (controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_B);
					bool x_button		= (controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_X);
					bool y_button		= (controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_Y);
					
					uint8_t left_trigger	= controller_state.Gamepad.bLeftTrigger;
					uint8_t right_trigger	= controller_state.Gamepad.bRightTrigger;
					
					uint16_t thumb_left_x	= controller_state.Gamepad.sThumbLX;
					uint16_t thumb_left_y	= controller_state.Gamepad.sThumbLY;
					uint16_t thumb_right_x	= controller_state.Gamepad.sThumbRX;
					uint16_t thumb_right_y	= controller_state.Gamepad.sThumbRY;
				}
				else	// Controller is not connected 
				{
					
				}
			}

			// get keyboard input

		}
	}
	else
	{
		OutputDebugStringA("Window handle is null\n");
	}
	
	return 0;
}