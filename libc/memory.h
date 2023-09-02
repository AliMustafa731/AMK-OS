#ifndef memory_included
#define memory_included

#include <libc/stdef.h>

extern void memcpy(uint8_t* dest, uint8_t* src, size_t size);

extern void memset(uint8_t* ptr, uint8_t val, size_t size);

#endif  // memory_included

