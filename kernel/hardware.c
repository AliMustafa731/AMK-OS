
#include <kernel/hardware.h>
#include <kernel/idt.h>
#include <kernel/gdt.h>

void Hardware_init()
{
	GDT_init();
	IDT_init(0x8);
	PIC_init();
	Timer_init();
}

void PIC_init()  // Programable Interrupt Controller
{
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

void Timer_init()  // Programable Interval Timer
{
	
}

//------------------------------------
//  Accessing the mapped I/O ports
//------------------------------------
uint8_t port_byte_in(uint16_t port)
{
    uint8_t result;

    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));

    return result;
}

void port_byte_out(uint16_t port, uint8_t data)
{
    __asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}

uint16_t port_word_in(uint16_t port)
{
    uint16_t result;

    __asm__("in %%dx, %%ax" : "=a" (result) : "d" (port));

    return result;
}

void port_word_out(uint16_t port, uint16_t data)
{
    __asm__("out %%ax, %%dx" : : "a" (data), "d" (port));
}
