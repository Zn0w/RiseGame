#include "../utils/file_io.h"

#include <windows.h>
#include <stdint.h>
#include <assert.h>


void* debug_platform_read_file(char* filepath)
{
	void* result = 0;
	
	HANDLE file_handle = CreateFile(filepath, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	if (file_handle != INVALID_HANDLE_VALUE)
	{
		LARGE_INTEGER file_size;
		if (GetFileSizeEx(file_handle, &file_size))
		{
			// assert that a file size is less or equals to the maximum 32-bit value
			assert(file_size.QuadPart <= 0xFFFFFFFF);
			
			result = VirtualAlloc(0, (uint32_t)file_size.QuadPart, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
			if (result)
			{
				DWORD bytes_read;
				if (ReadFile(file_handle, result, file_size.QuadPart, &bytes_read, 0) &&
						(bytes_read == (uint32_t)file_size.QuadPart))
				{
					OutputDebugStringA("Everything is in order\n");
				}
				else
				{
					close_file(result);
					result = 0;
				}
			}
		}
		
		CloseHandle(file_handle);
	}

	return result;
}

void debug_platform_write_file(char* filepath, uint32_t memory_size, void* memory)
{
	
}

void close_file(void* memory)
{
	VirtualFree(memory, 0, MEM_RELEASE);
}