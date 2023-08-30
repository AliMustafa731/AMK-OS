#ifndef gdt_included
#define gdt_included

#include <libc/stdef.h>

//-----------------------------
//  Global Descriptor Table
//-----------------------------

/***	 gdt descriptor access bit flags.	***/

//! set access bit
#define GDT_DESC_ACCESS			0x01	//00000001

//! descriptor is readable and writable. default: read only
#define GDT_DESC_READWRITE		0x02	//00000010

//! set expansion direction bit
#define GDT_DESC_EXPANSION		0x04	//00000100

//! executable code segment. Default: data segment
#define GDT_DESC_EXEC_CODE		0x08	//00001000

//! set code or data descriptor. defult: system defined descriptor
#define GDT_DESC_CODEDATA		0x10	//00010000

//! set dpl bits
#define GDT_DESC_DPL			0x60	//01100000

//! set "in memory" bit
#define GDT_DESC_MEMORY			0x80	//10000000

/**	gdt descriptor grandularity bit flags	***/

//! masks out limitHi (High 4 bits of limit)
#define GDT_GRAND_LIMITHI_MASK	0x0F	//00001111

//! set os defined bit
#define GDT_GRAND_OS			0x10	//00010000

//! set if 32bit. default: 16 bit
#define GDT_GRAND_32BIT			0x40	//01000000

//! 4k grandularity. default: none
#define GDT_GRAND_4K			0x80	//10000000

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


GDT_Descriptor_t* GDT_get_descriptor(uint16_t index);

void GDT_set_descriptor(uint16_t index, uint32_t base_address, uint32_t limit, uint8_t access, uint8_t granuality);

void GDT_init();


#endif  // gdt_included
