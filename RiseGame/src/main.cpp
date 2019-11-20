#include <windows.h>
#include <stdint.h>
#include <stdio.h>
#include <dsound.h>
#include <malloc.h>

#include "input/gamepad.h"
#include "game.h"


struct Win32_BitmapBuffer
{
	int width, height;
	BITMAPINFO info;
	void* memory;
	int pitch;	// in bytes
};

struct WindowDimensions
{
	int width, height;
};

#define BYTES_PER_PIXEL 4


static bool running;
static Win32_BitmapBuffer backbuffer;


WindowDimensions getWindowDimensions(HWND window_handle)
{
	RECT client_rect;
	GetClientRect(window_handle, &client_rect);

	return { client_rect.right - client_rect.left, client_rect.bottom - client_rect.top };
}


// resizes the Device Independent Buffer (DIB) section
static void win32_resizeFrameBuffer(Win32_BitmapBuffer* buffer, int width, int height)
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

	int bitmap_memory_size = buffer->width * buffer->height * BYTES_PER_PIXEL;
	buffer->memory = VirtualAlloc(0, bitmap_memory_size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	buffer->pitch = buffer->width * BYTES_PER_PIXEL;
}

static void win32_copyBufferToWindow(Win32_BitmapBuffer* buffer, HDC device_context, int window_width, int window_height)
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

//----------------------------------------------DIRECT-----SOUND-------------------------------------------------------------------

struct Win32_SoundOutput
{
	int samples_per_second;
	int bytes_per_sample;
	int sample_hz;
	int16_t sample_volume;
	uint32_t sample_index;
	int wave_period;
	int sound_buffer_size;
	int latency_sample_count;
};


static LPDIRECTSOUNDBUFFER win32_sound_buffer;


// Load DirectSound library
typedef HRESULT WINAPI dsound_create(LPCGUID pcGuidDevice, LPDIRECTSOUND* ppDS, LPUNKNOWN pUnkOuter);

void win32_InitDirectSound(HWND window_handle, uint32_t samples_per_sec, uint32_t sound_buffer_size_in_bytes)
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

				if (SUCCEEDED(direct_sound_object->CreateSoundBuffer(&buffer_description, &win32_sound_buffer, 0)))
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

static void win32_clearSoundBuffer(Win32_SoundOutput* sound_output)
{
	void* region_1;
	DWORD region_1_size;
	void* region_2;
	DWORD region_2_size;
	
	if (SUCCEEDED(win32_sound_buffer->Lock(0, sound_output->sound_buffer_size,
		&region_1, &region_1_size,
		&region_2, &region_2_size,
		0
	)))
	{
		DWORD region_1_sample_count = region_1_size / sound_output->bytes_per_sample;

		int16_t* dest_sample = (int16_t*)region_1;

		for (DWORD i = 0; i < region_1_sample_count; ++i)
		{
			*dest_sample++ = 0;
			*dest_sample++ = 0;

			sound_output->sample_index++;
		}

		dest_sample = (int16_t*)region_2;
		DWORD region_2_sample_count = region_2_size / sound_output->bytes_per_sample;

		for (DWORD i = 0; i < region_2_sample_count; ++i)
		{
			*dest_sample++ = 0;
			*dest_sample++ = 0;

			sound_output->sample_index++;
		}

		win32_sound_buffer->Unlock(region_1, region_1_size, region_2, region_2_size);
	}
}

static void win32_fillSoundBuffer(SoundBuffer* sound_buffer, Win32_SoundOutput* sound_output, DWORD byte_to_lock, DWORD bytes_to_write)
{
	void* region_1;
	DWORD region_1_size;
	void* region_2;
	DWORD region_2_size;

	if (SUCCEEDED(win32_sound_buffer->Lock(byte_to_lock, bytes_to_write,
		&region_1, &region_1_size,
		&region_2, &region_2_size,
		0
	)))
	{
		// TODO : assert that region_1_size and region_2_size are valid

		DWORD region_1_sample_count = region_1_size / sound_output->bytes_per_sample;

		int16_t* dest_sample = (int16_t*)region_1;
		int16_t* source_sample = sound_buffer->samples_buffer;

		for (DWORD i = 0; i < region_1_sample_count; ++i)
		{
			*dest_sample++ = *source_sample++;
			*dest_sample++ = *source_sample++;

			sound_output->sample_index++;
		}

		dest_sample = (int16_t*)region_2;
		DWORD region_2_sample_count = region_2_size / sound_output->bytes_per_sample;

		for (DWORD i = 0; i < region_2_sample_count; ++i)
		{
			*dest_sample++ = *source_sample++;
			*dest_sample++ = *source_sample++;

			sound_output->sample_index++;
		}

		win32_sound_buffer->Unlock(region_1, region_1_size, region_2, region_2_size);
	}
}
//---------------------------------------------------------------------------------------------------------------------------

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
			win32_copyBufferToWindow(&backbuffer, device_context, window_dimensions.width, window_dimensions.height);
			
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
	
	win32_resizeFrameBuffer(&backbuffer, screen_width, screen_height);

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
		// DirectSound output test (sine wave)d
		Win32_SoundOutput sound_output;
		sound_output.samples_per_second = 48000;
		sound_output.bytes_per_sample = sizeof(int16_t) * 2;
		sound_output.sample_hz = 256;
		sound_output.sample_volume = 400;
		sound_output.sample_index = 0;
		sound_output.wave_period = sound_output.samples_per_second / sound_output.sample_hz;
		sound_output.sound_buffer_size = sound_output.samples_per_second * sound_output.bytes_per_sample;
		sound_output.latency_sample_count = sound_output.samples_per_second / 12;
		
		win32_InitDirectSound(window_handle, sound_output.samples_per_second, sound_output.sound_buffer_size);
		win32_clearSoundBuffer(&sound_output);
		win32_sound_buffer->Play(0, 0, DSBPLAY_LOOPING);

		int16_t* samples = (int16_t*) VirtualAlloc(0, sound_output.sound_buffer_size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

		running = true;
		
		LARGE_INTEGER cpu_frequency;
		QueryPerformanceFrequency(&cpu_frequency);
		
		LARGE_INTEGER last_counter;
		QueryPerformanceCounter(&last_counter);

		uint64_t last_cycle_count = __rdtsc();

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
			GamepadInputMap gamepad_input;
			getGamepadInput(&gamepad_input);
			
			BitmapBuffer graphics_buffer = {};
			graphics_buffer.memory = backbuffer.memory;
			graphics_buffer.width = backbuffer.width;
			graphics_buffer.height = backbuffer.height;
			graphics_buffer.pitch = backbuffer.pitch;
			

			bool sound_is_valid = false;
			DWORD play_cursor_position;
			DWORD write_cursor_position;
			DWORD byte_to_lock;
			DWORD target_cursor_position;
			DWORD bytes_to_write;
			if (SUCCEEDED(win32_sound_buffer->GetCurrentPosition(&play_cursor_position, &write_cursor_position)))
			{
				byte_to_lock = (sound_output.sample_index * sound_output.bytes_per_sample) % sound_output.sound_buffer_size;
				target_cursor_position = ((play_cursor_position + (sound_output.latency_sample_count * sound_output.bytes_per_sample))
											% sound_output.sound_buffer_size);
				bytes_to_write = 0;
				if (byte_to_lock > target_cursor_position)
				{
					bytes_to_write = (sound_output.sound_buffer_size - byte_to_lock) + target_cursor_position;
				}
				else
				{
					bytes_to_write = target_cursor_position - byte_to_lock;
				}
				
				sound_is_valid = true;
			}
			
			SoundBuffer sound_buffer = {};
			sound_buffer.samples_per_second = sound_output.samples_per_second;
			sound_buffer.sample_count = bytes_to_write / sound_output.bytes_per_sample;
			sound_buffer.samples_buffer = samples;
			
			game_update_and_render(1.0f, &graphics_buffer, &sound_buffer);

			// DirectSound output test (sine wave)
			if (sound_is_valid)
			{
				win32_fillSoundBuffer(&sound_buffer, &sound_output, byte_to_lock, bytes_to_write);
			}


			HDC device_context = GetDC(window_handle);
			WindowDimensions dimensions = getWindowDimensions(window_handle);
			win32_copyBufferToWindow(
				&backbuffer,
				device_context,
				dimensions.width,
				dimensions.height
			);
			ReleaseDC(window_handle, device_context);


			uint64_t end_cycle_count = __rdtsc();
			
			LARGE_INTEGER end_counter;
			QueryPerformanceCounter(&end_counter);

			uint64_t cycles_elapsed = end_cycle_count - last_cycle_count;
			int64_t counter_elapsed = end_counter.QuadPart - last_counter.QuadPart;
			
			float elapsed_time = 1000.0f * (float)counter_elapsed / (float)cpu_frequency.QuadPart;	// in milliseconds
			int32_t fps = (int32_t)(cpu_frequency.QuadPart / counter_elapsed);
			
			char char_buffer[256];
			wsprintf(char_buffer, "fps: %d\n", fps);
			OutputDebugStringA(char_buffer);
			
			// TODO : pass delta to the game function and (or) display fps

			last_cycle_count = end_cycle_count;
			last_counter = end_counter;
		}

		game_destroy();
	}
	else
	{
		OutputDebugStringA("Window handle is null\n");
	}
	
	return 0;
}