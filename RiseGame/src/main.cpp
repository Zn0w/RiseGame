#include <windows.h>
#include <stdint.h>
#include <xinput.h>
#include <dsound.h>
#include <math.h>

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

struct SoundOutput
{
	int samples_per_second;
	int bytes_per_sample;
	int sample_hz;
	int16_t sample_volume;
	uint32_t sample_index;
	int wave_period;
	int direct_sound_buffer_size;
};

#define SIN_PERIOD (2.0f * 3.14159265359f)


static bool running;
static BitmapBuffer backbuffer;
static LPDIRECTSOUNDBUFFER direct_sound_buffer;


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
		XInputGetState = (xinput_get_state*)GetProcAddress(XInputLibrary, "XInputGetState");
		XInputSetState = (xinput_set_state*)GetProcAddress(XInputLibrary, "XInputSetState");
	}
	else
		OutputDebugStringA("Failed to load an XInput library\n");
}
//-----------------------------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------------------------
// Load DirectSound library
typedef HRESULT WINAPI dsound_create(LPCGUID pcGuidDevice, LPDIRECTSOUND* ppDS, LPUNKNOWN pUnkOuter);

static void InitDirectSound(HWND window_handle, uint32_t samples_per_sec, uint32_t sound_buffer_size_in_bytes)
{
	// load the DirectSound library
	HMODULE DirectSoundLibrary = LoadLibrary("dsound.dll");
	if (DirectSoundLibrary)
	{
		dsound_create* DirectSoundCreate = (dsound_create*)GetProcAddress(DirectSoundLibrary, "DirectSoundCreate");

		// get a DirectSound Object
		LPDIRECTSOUND direct_sound_object;
		if (DirectSoundCreate && SUCCEEDED(DirectSoundCreate(0, &direct_sound_object, 0)))
		{
			if (SUCCEEDED(direct_sound_object->SetCooperativeLevel(window_handle, DSSCL_PRIORITY)))
			{
				WAVEFORMATEX wave_format = {};
				wave_format.wFormatTag = WAVE_FORMAT_PCM;
				wave_format.nChannels = 2;	// stereo
				wave_format.nSamplesPerSec = samples_per_sec;
				wave_format.wBitsPerSample = 16;
				wave_format.nBlockAlign = (wave_format.nChannels * wave_format.wBitsPerSample) / 8;
				wave_format.nAvgBytesPerSec = wave_format.nSamplesPerSec * wave_format.nBlockAlign;
				wave_format.cbSize = 0;

				// "create" a primary buffer
				// NOTE : the primary buffer is not used as a buffer, but rather as a way to set a wave format
				//		  it will not be used further
				DSBUFFERDESC buffer_description = {};	// init all the members to 0
				buffer_description.dwSize = sizeof(buffer_description);
				buffer_description.dwFlags = DSBCAPS_PRIMARYBUFFER;

				LPDIRECTSOUNDBUFFER primary_buffer;
				if (SUCCEEDED(direct_sound_object->CreateSoundBuffer(&buffer_description, &primary_buffer, 0)))
				{
					if (SUCCEEDED(primary_buffer->SetFormat(&wave_format)))
					{
						OutputDebugStringA("DirectSound library loading status: Primary buffer format was set\n");
					}
					else
					{
						OutputDebugStringA("DirectSound library loading status: Failed to set a primary buffer format\n");
					}
				}
				else
					OutputDebugStringA("DirectSound library loading status: Failed to create a primary buffer\n");

				// "create" a secondary buffer
				// NOTE : the secodanry buffer is the actual buffer where the "sound" will be temporarily stored and played from
				//		  (in this case ~2 seconds of audio)
				buffer_description = {};
				buffer_description.dwSize = sizeof(buffer_description);
				buffer_description.dwFlags = 0;
				buffer_description.dwBufferBytes = sound_buffer_size_in_bytes;
				buffer_description.lpwfxFormat = &wave_format;

				if (SUCCEEDED(direct_sound_object->CreateSoundBuffer(&buffer_description, &direct_sound_buffer, 0)))
				{
					OutputDebugStringA("DirectSound library loading status: Secondary buffer was created successfully\n");
				}
				else
					OutputDebugStringA("DirectSound library loading status: Failed to create a secondary buffer\n");
			}
			else
				OutputDebugStringA("DirectSound library loading status: Failed to set cooperative level\n");
		}
		else
			OutputDebugStringA("DirectSound library loading status: Failed to get a DirectSound object\n");
	}
	else
		OutputDebugStringA("Failed to load DirectSound library\n");
}
//-----------------------------------------------------------------------------------------------------------------

void fillSoundBuffer(SoundOutput* sound_output, DWORD byte_to_lock, DWORD bytes_to_write)
{
	void* region_1;
	DWORD region_1_size;
	void* region_2;
	DWORD region_2_size;

	if (SUCCEEDED(direct_sound_buffer->Lock(byte_to_lock, bytes_to_write,
		&region_1, &region_1_size,
		&region_2, &region_2_size,
		0
	)))
	{
		// TODO : assert that region_1_size and region_2_size are valid

		int16_t* sample_out = (int16_t*)region_1;
		DWORD region_1_sample_count = region_1_size / sound_output->bytes_per_sample;

		for (DWORD i = 0; i < region_1_sample_count; ++i)
		{
			float t = SIN_PERIOD * (float)sound_output->sample_index / (float)sound_output->wave_period;
			float sin_value = sinf(t);
			int16_t sample_value = (int16_t)(sin_value * sound_output->sample_volume);

			*sample_out++ = sample_value;
			*sample_out++ = sample_value;

			sound_output->sample_index++;
		}

		sample_out = (int16_t*)region_2;
		DWORD region_2_sample_count = region_2_size / sound_output->bytes_per_sample;

		for (DWORD i = 0; i < region_2_sample_count; ++i)
		{
			float t = SIN_PERIOD * (float)sound_output->sample_index / (float)sound_output->wave_period;
			float sin_value = sinf(t);
			int16_t sample_value = (int16_t)(sin_value * sound_output->sample_volume);
			
			*sample_out++ = sample_value;
			*sample_out++ = sample_value;

			sound_output->sample_index++;
		}

		direct_sound_buffer->Unlock(region_1, region_1_size, region_2, region_2_size);
	}
}

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
		SoundOutput sine_wave_output;
		sine_wave_output.samples_per_second = 48000;
		sine_wave_output.bytes_per_sample = sizeof(int16_t) * 2;
		sine_wave_output.sample_hz = 256;
		sine_wave_output.sample_volume = 400;
		sine_wave_output.sample_index = 0;
		sine_wave_output.wave_period = sine_wave_output.samples_per_second / sine_wave_output.sample_hz;
		sine_wave_output.direct_sound_buffer_size = sine_wave_output.samples_per_second * sine_wave_output.bytes_per_sample;
		
		InitDirectSound(window_handle, sine_wave_output.samples_per_second, sine_wave_output.direct_sound_buffer_size);
		fillSoundBuffer(&sine_wave_output, 0, sine_wave_output.direct_sound_buffer_size);
		direct_sound_buffer->Play(0, 0, DSBPLAY_LOOPING);

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
					struct {
						uint8_t up : 1;	// the size of this member is 1 bit
						uint8_t down : 1;
						uint8_t left : 1;
						uint8_t right : 1;
						uint8_t start : 1;
						uint8_t back : 1;
						uint8_t left_thumb : 1;
						uint8_t right_thumb : 1;
						uint8_t left_shoulder : 1;
						uint8_t right_shoulder : 1;
						uint8_t a_button : 1;
						uint8_t b_button : 1;
						uint8_t x_button : 1;
						uint8_t y_button : 1;
					} buttons;
					
					buttons.up				= (controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP);
					buttons.down			= (controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
					buttons.left			= (controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
					buttons.right			= (controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
					buttons.start			= (controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_START);
					buttons.back			= (controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK);
					buttons.left_thumb		= (controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB);
					buttons.right_thumb	= (controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB);
					buttons.left_shoulder	= (controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER);
					buttons.right_shoulder = (controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER);
					buttons.a_button		= (controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_A);
					buttons.b_button		= (controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_B);
					buttons.x_button		= (controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_X);
					buttons.y_button		= (controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_Y);
					
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

			// DirectSound output test (square wave)

			DWORD play_cursor_position;
			DWORD write_cursor_position;
			if (SUCCEEDED(direct_sound_buffer->GetCurrentPosition(&play_cursor_position, &write_cursor_position)))
			{
				DWORD byte_to_lock = (sine_wave_output.sample_index * sine_wave_output.bytes_per_sample) % sine_wave_output.direct_sound_buffer_size;
				DWORD bytes_to_write = 0;
				if (byte_to_lock > play_cursor_position)
				{
					bytes_to_write = (sine_wave_output.direct_sound_buffer_size - byte_to_lock) + play_cursor_position;
				}
				else
				{
					bytes_to_write = play_cursor_position - byte_to_lock;
				}

				fillSoundBuffer(&sine_wave_output, byte_to_lock, bytes_to_write);
			}
		}

		destroy_game();
	}
	else
	{
		OutputDebugStringA("Window handle is null\n");
	}
	
	return 0;
}