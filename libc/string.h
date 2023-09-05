#ifndef string_included
#define string_included

#include <libc/stdint.h>

extern size_t strlen(uint8_t *str);

extern void strcpy(uint8_t *str1, uint8_t *str2);

extern void memcpy(void* dest, void* src, size_t size);

extern void memset(void* ptr, uint8_t val, size_t size);

extern void int_to_string(uint32_t val, uint8_t* buffer);

#endif  // string_included
