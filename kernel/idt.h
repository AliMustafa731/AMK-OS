#ifndef idt_included
#define idt_included

#include <libc/stdef.h>

//-------------------------------
//  Interrupt Descriptor Table
//-------------------------------

#define IDT_DESC_16_BIT       0x06 // 0000 0110
#define IDT_DESC_32_BIT       0x0E // 0000 1110
#define IDT_DESC_RING_0       0x00 // 0000 0000
#define IDT_DESC_RING_1       0x20 // 0010 0000
#define IDT_DESC_RING_2       0x40 // 0100 0000
#define IDT_DESC_RING_3       0x60 // 0110 0000
#define IDT_DESC_SEG_PRESENT  0x80 // 1000 0000

typedef struct _IDT_Descriptor_t
{
	uint16_t BaseLow;
	uint16_t Segment;
	uint8_t Reserved; // must be zero
	uint8_t Flags;
	uint16_t BaseHigh;
	
} __attribute__((packed)) IDT_Descriptor_t;

typedef struct _IDT_Register_t
{
	uint16_t size;
	uint32_t address;
	
} __attribute__((packed)) IDT_Register_t;


IDT_Descriptor_t* IDT_get_ir(uint16_t index);

void IDT_install_ir(uint16_t index, uint32_t handler_address, uint16_t segment, uint8_t flags);

void IDT_init(uint16_t segment);


#endif  // idt_included
