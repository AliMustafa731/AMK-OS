#ifndef stdef_included
#define stdef_included

#ifndef NULL
#define NULL 0
#endif

#define HIGH_BYTE(x) ((x >> 8) & 0xFF)
#define LOW_BYTE(x) (x & 0xFF)
#define HIGH_WORD(x) ((x >> 16) & 0xFFFF)
#define LOW_WORD(x) (x & 0xFFFF)

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

#endif  // stdef_included
