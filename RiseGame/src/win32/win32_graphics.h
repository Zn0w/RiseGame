#pragma once

#include <windows.h>


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