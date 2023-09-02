
#include <libc/memory.h>

void memcpy(uint8_t* dest, uint8_t* src, size_t size)
{
	int i = 0;

	while (i < size) { dest[i] = src[i];  i++; }
}

void memset(uint8_t* ptr, uint8_t val, size_t size)
{
	int i = 0;

	while (i < size) { ptr[i] = val;  i++; }
}
