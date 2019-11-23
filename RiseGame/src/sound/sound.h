#pragma once

#include <stdint.h>
#include <math.h>


struct SoundBuffer
{
	int16_t* samples_buffer;
	int sample_count;
	int samples_per_second;
};


// TODO : get rid of this test code
#define SIN_PERIOD (2.0f * 3.14159265359f)


static void outputSound(SoundBuffer* sound_buffer, int16_t sample_volume, int sample_hz)
{
	static float t_sine;
	
	int wave_period = sound_buffer->samples_per_second / sample_hz;

	int16_t* sample_out = sound_buffer->samples_buffer;

	for (int sample_index = 0; sample_index < sound_buffer->sample_count; ++sample_index)
	{
		float sin_value = sinf(t_sine);
		int16_t sample_value = (int16_t)(sin_value * sample_volume);

		*sample_out++ = sample_value;
		*sample_out++ = sample_value;

		t_sine += SIN_PERIOD * 1.0f / (float)wave_period;
	}
}