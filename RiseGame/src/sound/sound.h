#pragma once

#include <stdint.h>
#include <math.h>


struct SoundBuffer
{
	int16_t* samples_buffer;
	int sample_count;
	int samples_per_second;
};

void outputSound(SoundBuffer* sound_buffer);

// TODO : get rid of this test code
#define SIN_PERIOD (2.0f * 3.14159265359f)