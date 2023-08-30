#ifndef memory_included
#define memory_included

#include <libc/stdef.h>

void memcpy(void* dest, void* src, size_t size);

void memset(void* ptr, uint8_t val, size_t size);

#endif  // memory_included

