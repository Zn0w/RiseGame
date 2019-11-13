#pragma once

#include <stdint.h>
#include <math.h>
// TODO : move this to a platform-dependent sound .cpp file
#include <dsound.h>


struct SoundOutput
{
	int samples_per_second;
	int bytes_per_sample;
	int sample_hz;
	int16_t sample_volume;
	uint32_t sample_index;
	int wave_period;
	int sound_buffer_size;
};

// TODO : get rid of this test code
#define SIN_PERIOD (2.0f * 3.14159265359f)


// TODO : wrap this into a platform-independent struct (e.g. SoundBuffer) or move to the .cpp file
static LPDIRECTSOUNDBUFFER direct_sound_buffer;

// TODO : make this platform-independent function (e.g. initSoundLibrary())
//-----------------------------------------------------------------------------------------------------------------
// Load DirectSound library
typedef HRESULT WINAPI dsound_create(LPCGUID pcGuidDevice, LPDIRECTSOUND* ppDS, LPUNKNOWN pUnkOuter);

void InitDirectSound(HWND window_handle, uint32_t samples_per_sec, uint32_t sound_buffer_size_in_bytes);
//-----------------------------------------------------------------------------------------------------------------

void fillSoundBuffer(SoundOutput* sound_output, DWORD byte_to_lock, DWORD bytes_to_write);

void startSoundPlay();

void loadSound(SoundOutput* sound_output);