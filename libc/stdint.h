#ifndef stdint_included
#define stdint_included

// standard type definitions
typedef char                int8_t;
typedef unsigned char       uint8_t;
typedef short               int16_t;
typedef unsigned short      uint16_t;
typedef int                 int32_t;
typedef unsigned int        uint32_t;
typedef long long           int64_t;
typedef unsigned long long  uint64_t;

typedef unsigned int        size_t;
typedef unsigned int        uintptr_t;
typedef int                 intptr_t;

#ifdef NULL
#undef NULL
#endif

#define NULL 0

#define HIGH_BYTE(x) ((x >> 8) & 0xFF)
#define LOW_BYTE(x) (x & 0xFF)
#define HIGH_WORD(x) ((x >> 16) & 0xFFFF)
#define LOW_WORD(x) (x & 0xFFFF)

#define INT8_MIN   0x80
#define INT16_MIN  0x8000
#define INT32_MIN  0x80000000
#define INT64_MIN  0x8000000000000000

#define INT8_MAX   0xEF
#define INT16_MAX  0xEFFF
#define INT32_MAX  0xEFFFFFFF
#define INT64_MAX  0xEFFFFFFFFFFFFFFF

#define UINT8_MIN   0x0
#define UINT16_MIN  0x0
#define UINT32_MIN  0x0
#define UINT64_MIN  0x0

#define UINT8_MAX   0xFF
#define UINT16_MAX  0xFFFF
#define UINT32_MAX  0xFFFFFFFF
#define UINT64_MAX  0xFFFFFFFFFFFFFFFF

#endif  // stdint_included
