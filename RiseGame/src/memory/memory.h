#pragma once

#include <stdint.h>


struct GameMemory
{
	bool is_initialized;

	uint64_t permanent_storage_size;
	void* permanent_storage;
	
	// NOTE : for storage of temporary data, can be initialized and freed multiple times
	uint64_t transient_storage_size;
	void* transient_storage;
};


#define Kilobytes(value) ((value) * 1024)
#define Megabytes(value) (Kilobytes(value) * 1024)
#define Gigabytes(value) (Megabytes(value) * 1024)