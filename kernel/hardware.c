
#include <kernel/hardware.h>
#include <kernel/idt.h>
#include <kernel/gdt.h>
#include <kernel/pic.h>

void Hardware_init()
{
	GDT_init;
	IDT_init(0x8);
	PIC_init(0x20, 0x28);
	Timer_init();
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
