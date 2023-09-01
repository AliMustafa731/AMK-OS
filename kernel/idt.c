
#include <kernel/idt.h>
#include <libc/memory.h>
#include <drivers/screen.h>
#include <drivers/ports.h>

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
	
	// remap the PIC to use interrupts numbers (32 to 48) for hardware interrupts
	// 0x20 : PIC 1 control register port
	// 0x21 : PIC 1 data register port
	// 0xA0 : PIC 2 control register port
	// 0xA1 : PIC 2 data register port
	
	// 0x11 = 00010001 binary. Enables initialization
	port_byte_out(0x20, 0x11);
	port_byte_out(0xA0, 0x11);
	
	port_byte_out(0x21, 0x20);  // map (IRQ 0 to IRQ 7)  to base 0x20 (32 decimal)
	port_byte_out(0xA1, 0x28);  // map (IRQ 8 to IRQ 15) to base 0x28 (40 decimal)
	
	// Set the IR line to connect both PICs
	port_byte_out(0x21, 0x4);  // 0x04 => 0100, second bit (IR line 2)
	port_byte_out(0xA1, 0x2);  // 0x02 => 0010, IR line 2
	
	// Set x86 mode, bit 0 enables 80x86 mode
	port_byte_out(0x21, 0x1);
	port_byte_out(0xA1, 0x1);
	
	// finished, null the data register
	port_byte_out(0x21, 0);
	port_byte_out(0xA1, 0);
}

