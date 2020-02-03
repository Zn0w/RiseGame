#pragma once

#include <stdint.h>

#include "../math/math.h"
#include "../utils/file_io.h"


struct BitmapBuffer
{
	int width, height;
	void* memory;
	int pitch;	// in bytes
};

struct RGBColor
{
	float red;
	float green;
	float blue;
};

struct Texture
{
	uint32_t* data;
};


static int bytes_per_pixel = 4;


static void render_background(BitmapBuffer* buffer, RGBColor color)
{
	uint32_t raw_color = (roundFloatToUInt32(color.red * 255.0f) << 16) |
		(roundFloatToUInt32(color.green * 255.0f) << 8) |
		(roundFloatToUInt32(color.blue * 255.0f) << 0);
	
	uint8_t* row = (uint8_t*)buffer->memory;
	for (int y = 0; y < buffer->height; y++)
	{
		uint32_t* pixel = (uint32_t*)row;
		for (int x = 0; x < buffer->width; x++)
		{
			*pixel++ = raw_color;
		}
		row += buffer->pitch;
	}
}

static void render_rectangle(BitmapBuffer* buffer, int32_t min_x, int32_t min_y, int32_t max_x, int32_t max_y, RGBColor color)
{
	if (min_x < 0)
		min_x = 0;
	
	if (min_y < 0)
		min_y = 0;

	if (max_x > buffer->width)
		max_x = buffer->width;

	if (max_y > buffer->height)
		max_y = buffer->height;

	uint32_t raw_color = (roundFloatToUInt32(color.red * 255.0f) << 16) |
						 (roundFloatToUInt32(color.green * 255.0f) << 8) |
						 (roundFloatToUInt32(color.blue * 255.0f) << 0);
	
	uint8_t* row = (uint8_t*)buffer->memory + min_x * bytes_per_pixel + min_y * buffer->pitch;
	for (int y = min_y; y < max_y; y++)
	{
		uint32_t* pixel = (uint32_t*)row;
		for (int x = min_x; x < max_x; x++)
		{
			*pixel++ = raw_color;
		}
		row += buffer->pitch;
	}
}

static Texture load_bmp_texture(char* filepath)
{
	void* file_contents = debug_platform_read_file(filepath);
	if (file_contents)
	{
#pragma pack(push, 1)	// exact fit, no padding (a message to fo the compiler to line up struct elements on 1 byte boundary, as opposed to default 4 bytes)
		struct bmp_file_header
		{
			uint16_t FileType;     /* File type, always 4D42h ("BM") */
			uint32_t FileSize;     /* Size of the file in bytes */
			uint16_t Reserved1;    /* Always 0 */
			uint16_t Reserved2;    /* Always 0 */
			uint32_t BitmapOffset; /* Starting position of image data in bytes */
			uint32_t Size;            /* Size of this header in bytes */
			int32_t Width;           /* Image width in pixels */
			int32_t Height;          /* Image height in pixels */
			uint16_t Planes;          /* Number of color planes */
			uint16_t BitsPerPixel;    /* Number of bits per pixel */
		};
#pragma pack(pop)	// revert to the default

		bmp_file_header* bmp = (bmp_file_header*)file_contents;
		uint32_t* pixels = (uint32_t*)((uint8_t*)file_contents + bmp->BitmapOffset);

		Texture texture = { pixels };
		return texture;

		// TODO : DO THAT WHEN RENDER RESOURCE IS NOT NEEDED ANYMORE
		//close_file(file_contents);
	}
	else
		return { 0 };

}

static void render_sprite(BitmapBuffer* buffer, int32_t min_x, int32_t min_y, int32_t max_x, int32_t max_y, Texture texture)
{
	if (min_x < 0)
		min_x = 0;

	if (min_y < 0)
		min_y = 0;

	if (max_x > buffer->width)
		max_x = buffer->width;

	if (max_y > buffer->height)
		max_y = buffer->height;

	int32_t pixel_width = max_x - min_x;
	int32_t pixel_height = max_y - min_y;

	uint32_t* source_row = texture.data + pixel_width * (pixel_height - 1);
	uint8_t* dest_row = (uint8_t*)buffer->memory;

	for (int y = min_y; y < max_y; y++)
	{
		uint32_t* dest = (uint32_t*)dest_row;
		uint32_t* source = source_row;
		
		for (int x = min_x; x < max_x; x++)
		{
			*dest++ = *source++;
		}

		dest_row += buffer->pitch;
		source_row -= pixel_width;
	}
}