
#include <libc/string.h>

size_t strlen(uint8_t *str)
{
    size_t len = 0;

    while(str[len] != 0) { len++; }

    return len;
}

void strcpy(uint8_t *str1, uint8_t *str2)
{
    while(*str1 != 0 && *str2 != 0)
    {
        *str1= *str2;
        str1++;
        str2++;
    }
}

void memcpy(void* dest, void* src, size_t size)
{
    uint8_t *_src = (uint8_t*)src;
    uint8_t *_dest = (uint8_t*)dest;
	int i = 0;

	while (i < size) { _dest[i] = _src[i];  i++; }
}

void memset(void* ptr, uint8_t val, size_t size)
{
    uint8_t *_ptr = (uint8_t*)ptr;
	int i = 0;

	while (i < size) { _ptr[i] = val;  i++; }
}

void int_to_string(uint32_t val, uint8_t* buffer)
{
    uint8_t _buffer[11];
    memset(buffer, 0, 11);
    memset(_buffer, 0, 11);
    
    int digits_count = 0;

    while(val != 0)
    {
        _buffer[digits_count] = (val % 10) + 0x30;
        val /= 10;

        digits_count++;
    }

    // reverse the order of digits
    for (int i = 0 ; i < digits_count ; i++) { buffer[digits_count - i - 1] = _buffer[i]; }
}

void hex_to_string(uint32_t val, uint8_t* buffer)
{
    uint8_t _buffer[9];
    memset(buffer, 0, 9);
    memset(_buffer, 0, 9);
    
    int digits_count = 0;

    while (val != 0)
    {
        _buffer[digits_count] = (val & 0xF) + 0x30;
        val /= 16;
        if (_buffer[digits_count] > 0x39) { _buffer[digits_count] += 7; }

        digits_count++;
    }

    // reverse the order of digits
    for (int i = 0 ; i < digits_count ; i++) { buffer[digits_count - i - 1] = _buffer[i]; }
}
