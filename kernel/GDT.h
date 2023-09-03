#ifndef gdt_included
#define gdt_included

#include <libc/stdint.h>

//-----------------------------
//  Global Descriptor Table
//-----------------------------

/***     gdt descriptor access bit flags.    ***/
#define GDT_DESC_ACCESS           0x01 //0000 0001   //! set access bit
#define GDT_DESC_READWRITE        0x02 //0000 0010   //! descriptor is readable and writable. default: read only
#define GDT_DESC_EXPANSION        0x04 //0000 0100   //! set expansion direction bit
#define GDT_DESC_EXEC_CODE        0x08 //0000 1000   //! executable code segment. Default: data segment
#define GDT_DESC_CODEDATA         0x10 //0001 0000   //! set code or data descriptor. defult: system defined descriptor
#define GDT_DESC_DPL              0x60 //0110 0000   //! set dpl bits
#define GDT_DESC_MEMORY           0x80 //1000 0000   //! set "in memory" bit

/**    gdt descriptor grandularity bit flags    ***/
#define GDT_GRAND_LIMITHI_MASK    0x0F //0000 1111   //! masks out limitHi (High 4 bits of limit)
#define GDT_GRAND_OS              0x10 //0001 0000   //! set os defined bit
#define GDT_GRAND_32BIT           0x40 //0100 0000   //! set if 32bit. default: 16 bit
#define GDT_GRAND_4K              0x80 //1000 0000   //! 4k grandularity. default: none

#define GDT_ENTRIES 5

typedef struct _GDT_Descriptor_t
{
    uint16_t Limit;
    uint16_t BaseLow;
    uint8_t BaseMid;
    uint8_t Access;
    uint8_t Granuality;
    uint8_t BaseHigh;
    
} __attribute__((packed)) GDT_Descriptor_t;

typedef struct _GDT_Register_t
{
    uint16_t size;
    uint32_t address;
    
} __attribute__((packed)) GDT_Register_t;


extern GDT_Descriptor_t* GDT_get_descriptor(uint16_t index);

extern void GDT_set_descriptor(uint16_t index, uint32_t base_address, uint32_t limit, uint8_t access, uint8_t granuality);

extern void GDT_init();


#endif  // gdt_included
