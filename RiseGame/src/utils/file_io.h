#pragma once

#include <stdint.h>


void* debug_platform_read_file(char* filepath);
void debug_platform_write_file(char* filepath, uint32_t memory_size, void* memory);
void close_file(void* memory);