
#include <libc/memory.h>

void memcpy(void* dest, void* src, size_t size)
{
	uint32_t chunks = size / 4;
	
	uint32_t *src_32 = (uint32_t*)src;
	uint32_t *dest_32 = (uint32_t*)dest;
	uint8_t *src_8 = (uint8_t*)src;
	uint8_t *dest_8 = (uint8_t*)dest;
	
	int i = 0;
	while (i < chunks)
	{
		dest_32[i] = src_32[i];
		i++;
	}
	
	i *= 4;
	
	while (i < size)
	{
		dest_8[i] = src_8[i];
		i++;
	}
}

void memset(void* ptr, uint8_t val, size_t size)
{
	uint8_t *ptr_8 = (uint8_t*)ptr;
	
	for (int i = 0 ; i < size ; i++)
	{
		ptr_8[i] = val;
	}
}
