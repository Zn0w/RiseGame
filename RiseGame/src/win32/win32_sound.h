#pragma once

#include <stdint.h>
#include <dsound.h>

#include "../game.h"


struct Win32_SoundOutput
{
	int samples_per_second;
	int bytes_per_sample;
	int sample_hz;
	int16_t sample_volume;
	uint32_t sample_index;
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