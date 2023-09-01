
#include <kernel/idt.h>
#include <libc/memory.h>
#include <drivers/screen.h>

#define IDT_MAX_INTERRUPTS 256

static IDT_Descriptor_t IDT_Table[IDT_MAX_INTERRUPTS];

static IDT_Register_t IDT_Register;

extern void ASM_IDT_Handler();

void IDT_Default_Handler()
{
	print("\nAn Interrupt was called\n");
}

IDT_Descriptor_t* IDT_get_ir(uint16_t index)
{
	if (index >= IDT_MAX_INTERRUPTS) { return NULL; }
	
	return &IDT_Table[index];
}

void IDT_install_ir(uint16_t index, uint32_t handler_address, uint16_t segment, uint8_t flags)
{
	if (index >= IDT_MAX_INTERRUPTS) { return; }
	
	memset(&IDT_Table[index], 0, sizeof(IDT_Descriptor_t));
	
	IDT_Table[index].BaseLow   =  (uint16_t)(handler_address & 0xFFFF);
	IDT_Table[index].BaseHigh  =  (uint16_t)((handler_address >> 16) & 0xFFFF);
	IDT_Table[index].Reserved  =  0;
	IDT_Table[index].Flags     =  flags;
	IDT_Table[index].Segment   =  segment;
}

void IDT_init(uint16_t segment)
{
	// set up IDTR register
	IDT_Register.size = (sizeof(IDT_Descriptor_t) * IDT_MAX_INTERRUPTS) - 1;
	IDT_Register.address = (uint32_t)&IDT_Table[0];
	
	memset(&IDT_Table[0], 0, sizeof(IDT_Descriptor_t) * IDT_MAX_INTERRUPTS);
	
	// register default handlers
	for (int i = 0 ; i < IDT_MAX_INTERRUPTS ; i++)
	{
		IDT_install_ir(i, (uint32_t)ASM_IDT_Handler, segment, IDT_DESC_32_BIT | IDT_DESC_SEG_PRESENT);
	}
	
	__asm__ __volatile__("lidtl (%0)" : : "r" (&IDT_Register));
}

