#include <windows.h>
#include <stdint.h>

#include "input/gamepad.h"
#include "sound/sound.h"
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
	buffer->memory = VirtualAlloc(0, bitmap_memory_size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

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

	int screen_width = 1280;
	int screen_height = 720;

	// TODO: apply the user-defined resolution
	// TODO: calculate x and y for a window origin to be in center
	
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
		// DirectSound output test (sine wave)
		SoundOutput sine_wave_output;
		sine_wave_output.samples_per_second = 48000;
		sine_wave_output.bytes_per_sample = sizeof(int16_t) * 2;
		sine_wave_output.sample_hz = 256;
		sine_wave_output.sample_volume = 400;
		sine_wave_output.sample_index = 0;
		sine_wave_output.wave_period = sine_wave_output.samples_per_second / sine_wave_output.sample_hz;
		sine_wave_output.sound_buffer_size = sine_wave_output.samples_per_second * sine_wave_output.bytes_per_sample;
		
		InitDirectSound(window_handle, sine_wave_output.samples_per_second, sine_wave_output.sound_buffer_size);
		fillSoundBuffer(&sine_wave_output, 0, sine_wave_output.sound_buffer_size);
		startSoundPlay();

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
			getGamepadInput();

			// DirectSound output test (sine wave)
			loadSound(&sine_wave_output);
		}

		destroy_game();
	}
	else
	{
		OutputDebugStringA("Window handle is null\n");
	}
	
	return 0;
}