
#include <libc/string.h>

size_t strlen(uint8_t *str)
{
    size_t len = 0;

    while(str[len] != 0)
    {
        len++;
    }

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

void int_to_string(uint32_t val, uint8_t* buffer)
{
    uint8_t _buffer[10];

    int num = 0;

    while(val != 0)
    {
        uint8_t digit = (val % 10) + 0x30;
        val /= 10;
        _buffer[num] = digit;
        num++;
    }

    // reverse the order of digits
    for (int i = 0 ; i < num ; i++) { buffer[num - i - 1] = _buffer[i]; }
}
