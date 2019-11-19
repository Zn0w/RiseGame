#include "sound.h"


void outputSound(SoundBuffer* sound_buffer)
{
	static float t_sine;
	int16_t sample_volume = 500;
	int sample_hz = 256;
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